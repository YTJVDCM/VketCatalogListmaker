Set obj = Wscript.CreateObject("Shell.Application")
if Wscript.Arguments.Count = 0 then
obj.ShellExecute "wscript.exe", WScript.ScriptFullName & " runas", "", "runas", 1
Wscript.Quit
end if
 
Wscript.Echo "Ç±Ç±ÇÕä«óùé“å†å¿Ç≈é¿çsÇ≥ÇÍÇ‹Ç∑"

Dim Ws
Set Ws = WScript.CreateObject("WScript.Shell")

Es.RegWrite, REG_SZ, HKEY_LOCAL_MACHINE, SOFTWARE\RegisteredApplications, VketURLImporter, SOFTWARE\VketURLImporter\Capabilities