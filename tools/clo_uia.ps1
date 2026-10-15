# Drives CLO3D windows through UI Automation for what its Python API cannot reach (the Avatar Editor sizes).
# Modes: click LABEL [TYPE] | set LABEL VALUE (the spinner on the label's row) | fields | dump [FILTER]. Windows only.
param([string]$Mode, [string]$Arg1 = "", [string]$Arg2 = "")
Add-Type -AssemblyName UIAutomationClient, UIAutomationTypes
Add-Type @"
using System; using System.Runtime.InteropServices;
public class M { [DllImport("user32.dll")] public static extern bool SetCursorPos(int x,int y);
[DllImport("user32.dll")] public static extern void mouse_event(int f,int x,int y,int d,int e);
[DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
[DllImport("user32.dll")] public static extern bool SetProcessDPIAware(); }
"@
[M]::SetProcessDPIAware() | Out-Null
$A=[System.Windows.Automation.AutomationElement]; $T=[System.Windows.Automation.TreeScope]; $CT=[System.Windows.Automation.ControlType]
$proc = Get-Process | Where-Object { $_.ProcessName -like "CLO_Standalone*" } | Select-Object -First 1
$pc = New-Object System.Windows.Automation.PropertyCondition($A::ProcessIdProperty, $proc.Id)
function Wins { $A::RootElement.FindAll($T::Children, $pc) }
function Click($el) { $r=$el.Current.BoundingRectangle; $x=[int]($r.X+$r.Width/2); $y=[int]($r.Y+$r.Height/2)
  [M]::SetCursorPos($x,$y) | Out-Null; Start-Sleep -Milliseconds 120; [M]::mouse_event(2,0,0,0,0); [M]::mouse_event(4,0,0,0,0); "clicked $($el.Current.Name) at $x,$y" }
function Find($name, $type) { foreach ($w in Wins) { $c = $w.FindAll($T::Descendants, [System.Windows.Automation.Condition]::TrueCondition)
  foreach ($e in $c) { if ($e.Current.Name -eq $name -and ($type -eq "" -or $e.Current.ControlType.ProgrammaticName -like "*$type")) { return $e } } } }
switch ($Mode) {
 "click" { $e = Find $Arg1 $Arg2; if ($e) { Click $e } else { "NOT FOUND $Arg1" } }
 "set" { # $Arg1 = label text (or combo value for Width/Height rows), $Arg2 = number. The spinner on the label's row.
   $all = @(); foreach ($w in Wins) { $all += $w.FindAll($T::Descendants, [System.Windows.Automation.Condition]::TrueCondition) }
   $lab = $all | Where-Object { ($_.Current.ControlType -eq $CT::Text -or $_.Current.ControlType -eq $CT::ComboBox) -and (
            $_.Current.Name -eq $Arg1 -or (($_.Current.ControlType -eq $CT::ComboBox) -and ($(try { $_.GetCurrentPattern([System.Windows.Automation.ValuePattern]::Pattern).Current.Value } catch { "" }) -eq $Arg1))) -and $_.Current.BoundingRectangle.X -gt 1590 } | Select-Object -First 1
   if (-not $lab) { "NOLABEL $Arg1"; break }
   $ly = $lab.Current.BoundingRectangle.Y; $lx = $lab.Current.BoundingRectangle.X
   $sp = $all | Where-Object { $_.Current.ControlType -eq $CT::Spinner -and [math]::Abs($_.Current.BoundingRectangle.Y - $ly) -lt 20 -and $_.Current.BoundingRectangle.X -gt $lx } |
         Sort-Object { $_.Current.BoundingRectangle.X } | Select-Object -First 1
   if (-not $sp) { "NOSPINNER $Arg1"; break }
   $vp = $sp.GetCurrentPattern([System.Windows.Automation.ValuePattern]::Pattern)
   $before = $vp.Current.Value; $vp.SetValue($Arg2); Start-Sleep -Milliseconds 200
   # A value set this way only reaches the body when the field is confirmed: focus it and press Enter, or the
   # last field set is never applied (seen live 2026-10-16: bust spinner 91.10, body 80.64).
   $sp.SetFocus(); Start-Sleep -Milliseconds 150
   (New-Object -ComObject WScript.Shell).SendKeys("{ENTER}"); Start-Sleep -Milliseconds 700
   "SET $Arg1 | $before -> $($vp.Current.Value)" }
 "fields" { foreach ($w in Wins) { foreach ($e in $w.FindAll($T::Descendants, [System.Windows.Automation.Condition]::TrueCondition)) {
     $t=$e.Current.ControlType.ProgrammaticName.Replace("ControlType.",""); $r=$e.Current.BoundingRectangle
     if ($r.X -lt 1590 -or $r.Y -lt 280) { continue }
     if ($t -in @("Text","Spinner","Edit","ComboBox","Button")) {
       $v=""; try { $v=$e.GetCurrentPattern([System.Windows.Automation.ValuePattern]::Pattern).Current.Value } catch {}
       $pats = ($e.GetSupportedPatterns() | ForEach-Object { $_.ProgrammaticName.Replace("PatternIdentifiers.Pattern","") }) -join ","
       "$t|$($e.Current.Name)|$([int]$r.X)|$([int]$r.Y)|$([int]$r.Width)|v=$v|$pats" } } } }
 "dump"  { foreach ($w in Wins) { "WINDOW " + $w.Current.Name + " | " + $w.Current.ClassName + " | " + $w.Current.BoundingRectangle
   foreach ($e in $w.FindAll($T::Descendants, [System.Windows.Automation.Condition]::TrueCondition)) {
     $n=$e.Current.Name; $t=$e.Current.ControlType.ProgrammaticName.Replace("ControlType.","")
     if ($Arg1 -eq "" -or $n -like "*$Arg1*" -or $t -like "*$Arg1*") { "  $t | $n | " + $e.Current.AutomationId + " | " + $e.Current.BoundingRectangle } } } }
}
