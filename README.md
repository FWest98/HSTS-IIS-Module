# HSTS IIS Module #

## Project Description ##
A module for IIS which enables HTTP Strict Transport Security compliant with the HSTS Draft Specification (RFC 6797).

## Justification ##
Whilst it is simple to add a custom header to an IIS site, there is no simple way to add the HSTS header in a way that is compliant with the draft specification (RFC 6797). Specifically from section 7.2:

_An HSTS Host MUST NOT include the STS header field in HTTP responses conveyed over non-secure transport._

An additional driver for such a module is the seriousness of attack vectors such as sslstrip. It is hoped that simplicity of installation and configuration will avoid any excuse for not implementing the most effective defence against such attacks. 
