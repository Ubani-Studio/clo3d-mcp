# Press a button in a CLO3D dialog through UI Automation (InvokePattern): no mouse, no focus change, so it works
# while other apps are in front. Searches every CLO process (stray extra instances included).
#   powershell -File tools\clo_dialog.ps1 -Title "Open Motion" -Button OK [-TimeoutSec 60]
#   powershell -File tools\clo_dialog.ps1 -Contains "Move Avatar" -Button OK     (dialog found by a text inside it)
#   powershell -File tools\clo_dialog.ps1 -Title "Open Motion" -List      (list the dialog's buttons)
# -Contains exists because CLO's Open Motion dialog draws its own title bar: UI Automation does not expose
# "Open Motion" as a name (2026-11-16), but its checkbox label is exposed. The button is looked for in the
# top-level window that holds the matching element.
# Exit codes: 0 pressed (or listed), 1 no such dialog within the timeout, 2 dialog found but no such button.
param([string]$Title = "", [string]$Contains = "", [string]$Button = "OK", [int]$TimeoutSec = 60, [switch]$List)
Add-Type -AssemblyName UIAutomationClient, UIAutomationTypes
$A = [System.Windows.Automation.AutomationElement]; $T = [System.Windows.Automation.TreeScope]
$CT = [System.Windows.Automation.ControlType]
$walker = [System.Windows.Automation.TreeWalker]::ControlViewWalker
$isButton = New-Object System.Windows.Automation.PropertyCondition($A::ControlTypeProperty, $CT::Button)

function Matches($name) {
  if ($Title -and $name -eq $Title) { return $true }
  if ($Contains -and $name -and $name.Contains($Contains)) { return $true }
  return $false
}
function Press($d, $pidv, $label) {
  $buttons = @($d.FindAll($T::Descendants, $isButton))
  if ($List) { "dialog '$label' (pid $pidv) buttons: " + (($buttons | ForEach-Object { "'" + $_.Current.Name + "'" }) -join ", "); exit 0 }
  foreach ($b in $buttons) {
    if ($b.Current.Name.Trim() -match "^\W*$([regex]::Escape($Button))\W*$") {
      $b.GetCurrentPattern([System.Windows.Automation.InvokePattern]::Pattern).Invoke()
      "pressed '$($b.Current.Name)' in '$label' (pid $pidv)"; exit 0
    }
  }
  if ($buttons.Count) { "found '$label' but no '$Button' button: " + (($buttons | ForEach-Object { $_.Current.Name }) -join ", "); exit 2 }
}

$label = if ($Title) { $Title } else { "*$Contains*" }
$deadline = (Get-Date).AddSeconds($TimeoutSec)
while ((Get-Date) -lt $deadline) {
  $ids = @(Get-Process | Where-Object { $_.ProcessName -like "CLO_Standalone*" } | ForEach-Object { $_.Id })
  foreach ($w in $A::RootElement.FindAll($T::Children, [System.Windows.Automation.Condition]::TrueCondition)) {
    $pidv = $w.Current.ProcessId
    if ($ids -notcontains $pidv) { continue }
    if (Matches $w.Current.Name) { Press $w $pidv $label }
    foreach ($e in $w.FindAll($T::Descendants, [System.Windows.Automation.Condition]::TrueCondition)) {
      if (-not (Matches $e.Current.Name)) { continue }
      # climb to the nearest ancestor that has buttons (the dialog), not the whole main window
      $d = $e
      while ($d -and -not $d.FindFirst($T::Descendants, $isButton)) { $d = $walker.GetParent($d) }
      if ($d) { Press $d $pidv $label }
    }
  }
  Start-Sleep -Milliseconds 500
}
"no '$label' dialog within $TimeoutSec s"; exit 1
