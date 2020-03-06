Set obj = Wscript.CreateObject("Shell.Application")
if Wscript.Arguments.Count = 0 then
obj.ShellExecute "wscript.exe", WScript.ScriptFullName & " runas", "", "runas", 1
Wscript.Quit
end if

Dim Ws
Set Ws = WScript.CreateObject("WScript.Shell")

RegWrite, "HKEY_LOCAL_MACHINE\SOFTWARE\RegisteredApplications","VketCatalogListMakers" , "REG_SZ"

Wscript.Echo "ŠÖ˜A•t‚¯İ’èŠ®—¹"