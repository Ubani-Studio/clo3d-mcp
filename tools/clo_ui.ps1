# Drive CLO3D's UI through UI Automation patterns (Invoke / ExpandCollapse / Value / SelectionItem): no mouse, no
# focus change, so it works while other apps are in front. Targets the CLO process whose window has the menu bar
# (stray CLO instances sitting at the sign-in screen are ignored).
#   clo_ui.ps1 menu   "Avatar>Auto Convert to Motion"          open a menu path and invoke the last item
#   clo_ui.ps1 press  "<dialog title>" "<button>"             press a button in a dialog (waits up to 60 s)
#   clo_ui.ps1 combo  "<dialog title>" "<item>"               choose an item in the dialog's (first) combo box
#   clo_ui.ps1 file   "<dialog title>" "<path>" "<button>"    Windows file dialog: type the file name, press Open/Save
#   clo_ui.ps1 list   "<dialog title>"                        list the dialog's controls (name | type)
param([string]$Action, [string]$A1 = "", [string]$A2 = "", [string]$A3 = "", [int]$TimeoutSec = 60)
Add-Type -AssemblyName UIAutomationClient, UIAutomationTypes
$AE = [System.Windows.Automation.AutomationElement]; $T = [System.Windows.Automation.TreeScope]
$CT = [System.Windows.Automation.ControlType]; $TRUE_ = [System.Windows.Automation.Condition]::TrueCondition
function Cond($prop, $val) { New-Object System.Windows.Automation.PropertyCondition($prop, $val) }
function Pattern($el, $p) { $el.GetCurrentPattern($p::Pattern) }

function MainClo {
  # CLO's main frame is a "TitleFrame" window; while a modal dialog is open its menu bar is not exposed, so the class
  # is what identifies it (stray instances at the sign-in screen have no TitleFrame)
  foreach ($p in (Get-Process | Where-Object { $_.ProcessName -like "CLO_Standalone*" })) {
    foreach ($w in $AE::RootElement.FindAll($T::Children, (Cond $AE::ProcessIdProperty $p.Id))) {
      if ($w.Current.ClassName -eq "TitleFrame") { return $w }
    }
  }
}
$main = MainClo
if (-not $main) { "no CLO window with a menu bar"; exit 3 }
$cloPid = $main.Current.ProcessId

function Windows { $AE::RootElement.FindAll($T::Children, (Cond $AE::ProcessIdProperty $cloPid)) }
function FindDialog($title) {
  $deadline = (Get-Date).AddSeconds($TimeoutSec)
  while ((Get-Date) -lt $deadline) {
    foreach ($w in Windows) {
      if ($w.Current.Name -eq $title) { return $w }
      $d = $w.FindFirst($T::Descendants, (Cond $AE::NameProperty $title))
      if ($d) { return $d }
    }
    Start-Sleep -Milliseconds 400
  }
}
function FindNamed($root, $name, $type) {
  foreach ($e in $root.FindAll($T::Descendants, (Cond $AE::ControlTypeProperty $type))) {
    if ($e.Current.Name.Trim() -match "^\W*$([regex]::Escape($name))\W*$") { return $e }
  }
}
function FindMenuItem($name) {   # popup menus are separate windows of the CLO process
  $deadline = (Get-Date).AddSeconds(10)
  while ((Get-Date) -lt $deadline) {
    foreach ($w in Windows) {
      $m = FindNamed $w $name $CT::MenuItem
      if ($m) { return $m }
    }
    Start-Sleep -Milliseconds 300
  }
}

switch ($Action) {
  "menu" {
    $parts = $A1.Split(">")
    $top = FindNamed $main $parts[0] $CT::MenuItem
    if (-not $top) { "menu '$($parts[0])' not found"; exit 1 }
    (Pattern $top ([System.Windows.Automation.ExpandCollapsePattern])).Expand(); Start-Sleep -Milliseconds 500
    for ($i = 1; $i -lt $parts.Count; $i++) {
      $it = FindMenuItem $parts[$i]
      if (-not $it) { "menu item '$($parts[$i])' not found"; exit 1 }
      if ($i -lt $parts.Count - 1) { (Pattern $it ([System.Windows.Automation.ExpandCollapsePattern])).Expand(); Start-Sleep -Milliseconds 400 }
      else { (Pattern $it ([System.Windows.Automation.InvokePattern])).Invoke(); "invoked $A1" }
    }
  }
  "press" {
    $d = FindDialog $A1; if (-not $d) { "no '$A1' dialog"; exit 1 }
    $b = FindNamed $d $A2 $CT::Button; if (-not $b) { "no '$A2' button in '$A1'"; exit 2 }
    (Pattern $b ([System.Windows.Automation.InvokePattern])).Invoke(); "pressed '$($b.Current.Name)' in '$A1'"
  }
  "combo" {
    $d = FindDialog $A1; if (-not $d) { "no '$A1' dialog"; exit 1 }
    $c = $d.FindFirst($T::Descendants, (Cond $AE::ControlTypeProperty $CT::ComboBox)); if (-not $c) { "no combo box"; exit 2 }
    # CLO's combo boxes expose their items directly (no ExpandCollapse): select the item; fall back to setting the value
    try { (Pattern $c ([System.Windows.Automation.ExpandCollapsePattern])).Expand(); Start-Sleep -Milliseconds 400 } catch {}
    $item = FindNamed $d $A2 $CT::ListItem
    if (-not $item) { foreach ($w in Windows) { $item = FindNamed $w $A2 $CT::ListItem; if ($item) { break } } }
    $done = $false
    if ($item) {
      foreach ($p in @([System.Windows.Automation.SelectionItemPattern], [System.Windows.Automation.InvokePattern])) {
        try {
          if ($p -eq [System.Windows.Automation.SelectionItemPattern]) { (Pattern $item $p).Select() } else { (Pattern $item $p).Invoke() }
          $done = $true; break
        } catch {}
      }
    }
    if (-not $done) { try { (Pattern $c ([System.Windows.Automation.ValuePattern])).SetValue($A2); $done = $true } catch {} }
    Start-Sleep -Milliseconds 400
    try { (Pattern $c ([System.Windows.Automation.ExpandCollapsePattern])).Collapse() } catch {}
    $v = ""; try { $v = (Pattern $c ([System.Windows.Automation.ValuePattern])).Current.Value } catch {}
    if (-not $v) { $v = $c.Current.Name }
    "combo in '$A1' -> '$A2' (selected: $done, now '$v')"
  }
  "file" {
    $d = $null; $deadline = (Get-Date).AddSeconds($TimeoutSec)
    while (-not $d -and (Get-Date) -lt $deadline) {   # native dialogs are top-level windows of the CLO process
      foreach ($w in $AE::RootElement.FindAll($T::Children, (Cond $AE::ProcessIdProperty $cloPid))) {
        if ($w.Current.Name -eq $A1) { $d = $w }
        else { $x = $w.FindFirst($T::Children, (Cond $AE::NameProperty $A1)); if ($x) { $d = $x } }
      }
      if (-not $d) { Start-Sleep -Milliseconds 400 }
    }
    if (-not $d) { "no '$A1' file dialog"; exit 1 }
    $edit = $null
    foreach ($e in $d.FindAll($T::Descendants, (Cond $AE::ControlTypeProperty $CT::Edit))) {
      if ($e.Current.Name -like "File name*") { $edit = $e; break }
    }
    if (-not $edit) { "no File name box"; exit 2 }
    (Pattern $edit ([System.Windows.Automation.ValuePattern])).SetValue($A2); Start-Sleep -Milliseconds 300
    $btn = $null
    foreach ($b in $d.FindAll($T::Children, (Cond $AE::ControlTypeProperty $CT::Button))) { if ($b.Current.Name -eq $A3) { $btn = $b } }
    if (-not $btn) { $btn = FindNamed $d $A3 $CT::Button }
    if (-not $btn) { "no '$A3' button"; exit 2 }
    (Pattern $btn ([System.Windows.Automation.InvokePattern])).Invoke(); "file dialog '$A1': $A2 -> $A3"
  }
  "list" {
    $d = FindDialog $A1; if (-not $d) { "no '$A1' dialog"; exit 1 }
    foreach ($e in $d.FindAll($T::Descendants, $TRUE_)) {
      $t = $e.Current.ControlType.ProgrammaticName.Replace("ControlType.", "")
      if ($t -in @("Button", "ComboBox", "Edit", "Text", "CheckBox", "ListItem")) { "$t | $($e.Current.Name)" }
    }
  }
  default { "actions: menu | press | combo | file | list" ; exit 4 }
}
