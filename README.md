#NOTICE#
Due to lack of time, this project is no longer maintained. If you are interested in taking over ownership of the project, please contact me.


# HSTS IIS Module #

## Project Description ##
A module for IIS which enables HTTP Strict Transport Security compliant with the HSTS Draft Specification (RFC 6797). As of version 2.0 the module can be configured to redirect insecure requests.

## Downloads & Documentation ##
[Downloads of the installers and binaries are available from the CodePlex project site](https://hstsiis.codeplex.com)

Documentation is made available [in the documentation folder](documentation) of this repository.  Documentation topics include [installation](documentation/Installation.md), [enabling HSTS](documentation/Enabling HSTS.md), and an assortment of [frequently asked questions](documentation/Frequently Asked Questions.md).

## Project Structure ##
The project is split into three components: module, manager, installer.

### module ###
The 'module' sub-project is the work horse of the project. Developed in C++ the output of this is the actual IIS module, which could be installed and used standalone without the other components. It is responsible for subscribing to the events, injecting the HSTS header and performing the redirect (if necessary).

### manager ###
The 'manager' sub-project is the extension to the IIS manager (inetmgr.exe) and is developed in C#. This component gives the user a graphical interface to the configuration options.

### installer ###
Unsuprisingly, this subject produces an MSI installer which handles the copies the DLLs and modifies the IIS configuration. It is developed using the [WIX toolset](http://wixtoolset.org/).

## Tooling ##
The project uses some unconventional (within the Microsoft development environment) tooling. Specifically, the use of [Gradle](http://www.gradle.org/). I strongly believe in reducing the number of hurdles to first time developers. Ideally, someone who has no prior knowledge or skills without any tools installed, should be able to download and compile the project. Paid development tools such as Visual Studio obviously work against this. Yes, Visual Studio Express is free, but it doesn't allow for the installation of plugins and therefore WIX would need to be compiled extrnally. I also believe that IDEs such as Visual Studio can often make the process of compiling appear to be almost magical and can be very opaque.

Gradle allows me to remove a lot of these hurdles. It comes out of the box with [C++ support](http://www.gradle.org/docs/current/userguide/nativeBinaries.html), although the user must have the Visual C++ compiler installed. It also has a powerful API which means that I can use it to compile the C# and WIX whilst taking advantage of features such as akipping od up-to-date tasks.

## Building and Compiling ##

### Prerequisites ###
If you would like to compile the extension for yourself you will need a few prerequisites intalled:
* Visal C++ Compiler - To build the 'module'
* .NET Framework 2.0 (must be 2.0) - To build the 'manager'
* IIS 7.0 or above - Contains some libraries required for the 'manager'
* WIX toolset - To build the 'installer'

### Build ###
To build the entire project:

1. Download the source code (using git, or as zip)
2. Open a command prompt
3. Change to the source directory
4. Run 'gradlew buildinstaller'

The 'buildinstaller' task will build all of the sub-components and then build the installer. It will output the installer to installer/build/bin.

### A note on Strong Name Signing ###
The manager DLL must be signed in order to be installed into the Global Assembly Cache. It is a security issue to publish the private key used to sign the DLL which presents a bit of an issue. I wanted to ensure that the build process was as simple as possible so the instructions above will build the extension using a key that is included in the project and is therefore insecure. If you wish to build it with your own strong name key use the following commands:

1. gradlew compileCS -Psnk=YourSNKFile.snk
2. Locate the public key token of the generated file (you can use 'sn.exe -T path/to/file.dll' in the .NET SDK)
3. gradlew buildinstaller -PmanagerHash=YourHash

Note: the official downloads of the project are signed with a secure key, not the one included in the project. It is sad that others cannot build exactly the same artifacts that are downloaded from the project site, but this is the best compromise I could come up with. 

## Justification ##
Whilst it is simple to add a custom header to an IIS site, there is no simple way to add the HSTS header in a way that is compliant with the draft specification (RFC 6797). Specifically from section 7.2:

_An HSTS Host MUST NOT include the STS header field in HTTP responses conveyed over non-secure transport._

An additional driver for such a module is the seriousness of attack vectors such as sslstrip. It is hoped that simplicity of installation and configuration will avoid any excuse for not implementing the most effective defence against such attacks.

## Thanks ##

Thanks to Phill from [Dionach](http://www.dionach.com/) for the fantastic [Strip Headers](https://github.com/Dionach/StripHeaders/) IIS extension which is, aside from a great extension, one of the best references for developing a native IIS module.

Thanks also to everyone that has taken the time to reported issues and suggest improvements.

Special thanks to @FWest98 for his commits.
