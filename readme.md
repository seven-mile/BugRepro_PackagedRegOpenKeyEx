
```cpp
auto KEY_PATH = LR"(SOFTWARE)";

wil::unique_hkey hkey;
check_win32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_PATH, REG_OPTION_BACKUP_RESTORE, KEY_QUERY_VALUE, hkey.put()));
```

This is very restrictive and but not rare bug, all of the conditions below must be fulfilled to repro:

* Packaged (full-trusted) apps with elevation, unpackaged won't trigger, and non elevated apps are expectedly denied.
* Open `HKLM\SOFTWARE` itself, any other siblings or subkeys like `HKLM\SYSTEM` or `HKLM\SOFTWARE\Microsoft\Windows` won't trigger.
* Open the key with the options REG_OPTION_BACKUP_RESTORE.

for a repro repo, check [this url](https://github.com/seven-mile/BugRepro_PackagedRegOpenKeyEx).


---

> **Note**
>
> You may want to debug the elevated packaged app. But due to a Visual Studio bug, we have to launch it and attatch the debugger later.
> 
> The ctor of MainWindow include the snippet below to wait until the debugger is attached.

```cpp
while (!IsDebuggerPresent())
  Sleep(100);
```
