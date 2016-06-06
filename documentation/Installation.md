# Installing the HSTS IIS Module
This software is compatible with IIS 7.0 and above.

## Using the installer
The HSTS installer can be found at the [releases](https://github.com/FWest98/hsts-iis-module/releases)

To install, follow the wizard's prompts.

## Manual installation
The option to manually install the module and the Manager plugin for IIS is made available for those who prefer to have fine-grained control over the changes made to the server.

### Module
**1.** Download Module DLL, Manager DLL, and Schema XML.  [Visit the download page](https://github.com/FWest98/hsts-iis-module/releases/latest) to download these three files.

**2.** Copy the *module* DLL (HstsIisModule.dll) to your inetsrv directory.  (%windir%\system32\inetsrv)

**3.** Copy the HSTS_Schema.xml file to the inetsrv\config\schema directory. (%windir%\system32\inetsrv\config\schema)

**4.** Locate applicationHost.config in the inetsrv\config directory. (%windir%\system32\inetsrv\config)

> **Make a backup of applicationHost.config file before proceeding.**

**5.** In applicationHost.config, locate the configuration/system.webServer/globalModules node, and add the following:

```xml
<add name="HstsIisModule" image="%windir%\System32\inetsrv\HstsIisModule.dll" />
```

##### IIS 7.x
**6.** In applicationHost.config, locate the configuration/location[path=""]/system.webServer/modules node, and add the following:

```xml
<add name="HstsIisModule" />
```

##### IIS 8.x
**6.** In applicationHost.config, locate the configuration/system.webServer/modules node, and add the following:

```xml
<add name="HstsIisModule" />
```

**7.** In applicationHost.config, locate the configuration/configSections/sectionGroup[name='system.webServer'] node, and add the following:

```xml
<section name="hsts" overrideModeDefault="Allow" />
```

This completes the installation of the Module.

### IIS Manager Plugin
**1.** Register the HSTS-IIS-Module.manager.dll file into the GAC.  The following example PowerShell script can be used to accomplish this step:

> Note:  You should run PowerShell as an Administrator for this script to work.

> Warning:  If you run this script verbatim, you will end up restarting IIS - This is required to enable the plugin, but make sure you've scheduled any necessary outages

```PowerShell
[System.Reflection.Assembly]::Load("System.EnterpriseServices, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a")            
$publish = New-Object System.EnterpriseServices.Internal.Publish            
$publish.GacInstall("c:\ReplaceWith\FolderPathTo\HSTS-IIS-Module.manager.dll")            

# Warning: iisreset will stop and start IIS, so only do this if you've planned to restart this service
iisreset
```

Alternatively, you could install the DLL into the GAC using gacutil.exe (if Visual Studio is installed on your server):

> Note:  You should open a command prompt window as an Administrator for this to work.

```
gacutil -i "c:\ReplaceWith\FolderPathTo\HSTS-IIS-Module.manager.dll"
```

**2.** Locate administration.config in the inetsrv\config directory. (%windir%\system32\inetsrv\config)

> **Make a backup of administration.config file before proceeding.**

**3.** In administration.config, locate the /configuration/moduleProviders node, and add the following:

```xml
<add name="hsts" type="HSTS_IIS_Module.Manager.ConfigModuleProvider, HSTS-IIS-Module.manager, Version=2.0.0.0, Culture=neutral, PublicKeyToken=26873d35e352ec07"/>
```

**4.** In administration.config, locate the configuration/location[path='.']/modules node, and add the following:

```xml
<add name="hsts" />
```

This completes the installation of the Manager plugin for IIS.
