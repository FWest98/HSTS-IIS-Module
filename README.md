**NOTE** that since Windows Server 2019, this feature is provided out-of-the-box in IIS, and this plugin is no longer necessary.

# HSTS IIS Module #

## Project Description ##
A module for IIS which enables HTTP Strict Transport Security compliant with the HSTS Draft Specification (RFC 6797). As of version 2.0 the module can be configured to redirect insecure requests.

## Downloads & Documentation ##
[Downloads of the installers and binaries are available from GitHub](https://github.com/FWest98/hsts-iis-module/releases)

Documentation is made available [in the documentation folder](documentation) of this repository.  Documentation topics include [installation](documentation/Installation.md), [enabling HSTS](documentation/Enabling HSTS.md), and an assortment of [frequently asked questions](documentation/Frequently Asked Questions.md).

## Project Structure ##
The project is split into three components: the module, the manager and the installer.

### HSTS IIS Module ###
This project is the work horse of the plug-in. Developed in C++ the output of this is the actual IIS module, which could be installed and used standalone without the other components. It is responsible for subscribing to the events, injecting the HSTS header and performing the redirect (if necessary).

### HSTS IIS Manager ###
The 'manager' project is the extension to the IIS manager (inetmgr.exe) and is developed in C#. This component gives the user a graphical interface to the configuration options.

### HSTS IIS Installer ###
Unsurprisingly, this subject produces an MSI installer which handles the copies the DLLs and modifies the IIS configuration. It is developed using the [WIX toolset](http://wixtoolset.org/).

## Tooling ##
This project uses the Visual Studio built-in tooling to compile, build and run C++/C# code. Using the WiX Plugin, the installer can also be built. The VS solution is currently compatible with version VS2015 and all binaries are compiled with the same version of VS. Any other combination of IDE/compiler might result in unexpected behaviour or other problems.

## Building and Compiling ##

### Prerequisites ###
If you would like to compile the extension for yourself you will need a few prerequisites intalled:
* Visual Studio 2015 (Community) - To build the module and the manager
* WiX Toolset plugin - To build the installer
* IIS 7.0 or above - Contains some dependencies of the manager that are required to build

### A note on Strong Name Signing ###
The manager DLL must be signed in order to be installed into the Global Assembly Cache. It is a security issue to publish the private key used to sign the DLL which presents a bit of an issue. I wanted to ensure that the build process was as simple as possible so the instructions above will build the extension using a key that is included in the project and is therefore insecure. If you wish to build it with your own strong name key, please modify the settings in Visual Studio to do so.

Note: the official downloads of the project are signed with a secure key, not the one included in the project. It is sad that others cannot build exactly the same artifacts that are downloaded from the project site, but this is the best compromise I could come up with. 

## Justification ##
Whilst it is simple to add a custom header to an IIS site, there is no simple way to add the HSTS header in a way that is compliant with the draft specification (RFC 6797). Specifically from section 7.2:

_An HSTS Host MUST NOT include the STS header field in HTTP responses conveyed over non-secure transport._

An additional driver for such a module is the seriousness of attack vectors such as sslstrip. It is hoped that simplicity of installation and configuration will avoid any excuse for not implementing the most effective defence against such attacks.

## Thanks ##

Thanks to Phill from [Dionach](http://www.dionach.com/) for the fantastic [Strip Headers](https://github.com/Dionach/StripHeaders/) IIS extension which is, aside from a great extension, one of the best references for developing a native IIS module.

Thanks to [Andrew Bancroft](https://github.com/andrewcbancroft) for his great work on improving this plugin and its documentation!

Thanks also to everyone that has taken the time to reported issues and suggest improvements.

Special thanks to Shane Argo for the great work he has done for the first versions of this plugin!
