# Dump ActivationContext data

This is simple tool to dump all information about ActivationContext stored in PEB.

To build and test code you will need **Visual Studio Build tools** 2013, 2015 or 2017, either standalone or one integrated with Visual Studio, and **cmake**:

* [Visual C++ 2015 Build tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)
* [Build Tools for Visual Studio 2017](https://www.visualstudio.com/downloads/#build-tools-for-visual-studio-2017)
* [cmake](https://cmake.org/)

To dump Activation  Context stored in **PEB.ActivationContextData**:
```
actctx.exe
```

To dump System Default Activation Context stored in **PEB.SystemDefaultActivationContextData**:

```
actctx.exe --system
```

**manifest.manifest** has various hacks to test the dumping tool, and shouldn't be considered for any real life scenarios.

Also note, that by manipulating ActivationContext data from the process. You may inject DLL into it, and perform hooking of COM classes for example. There are other entries that could be modified to load DLL from disk. DLL_REDIRECTION section is good candidate too. You may do this with other fields as well, just use your imagination.

From sample code there is COM class redirection of **CLSID_HTMLApplication** via **manifest.manifest** and call to **CreateInstance** will actually try to load **thisismyhtmldummydll.dll**. Of course, it will fail as DLL is not there, but it could be loaded if it was there.

Small bonus. Why is **gdiplus.dll** never loaded from the  **system32** folder? This is why:

```
pDllRedirection flags             : 0000000A
Key                               : GdiPlus.dll
pStringEntry->AssemblyRosterIndex : 00000003
pDllRedirection flags             : 00000002
Key                               : C:\Windows\system32\GdiPlus.dll
pStringEntry->AssemblyRosterIndex : 00000003
```

This tells us that if **gdiplus.dll** is loaded, by specifying only the DLL name (eg. LoadLibrary("gdiplus.dll")) or using full system32 path (eg. LoadLibrary("c:\\windows\\system32\\gdiplus.dll")) will always cause redirection to this **Assembly Information**:

```
AssemblyName       : Microsoft.Windows.GdiPlus
Assembly info size : 0000006C
Manifest Path      : C:\Windows\WinSxS\manifests\x86_microsoft.windows.gdiplus_6595b64144ccf1df_1.1.7601.23894_none_5c0be957a009922e.manifest
AssemblyDirectory  : x86_microsoft.windows.gdiplus_6595b64144ccf1df_1.1.7601.23894_none_5c0be957a009922e
RunLevel           : unspecified
uiAccess           : false
```
