# Enabling HSTS

## Enable for all applications under a Site in IIS
The following steps guide you to enable HSTS for all applications / websites under a Site in IIS.

**1.** In IIS, expand Sites and choose the one for which you would like to enable HSTS.

**2.** Under the 'Other' section, find the HTTP Strict Transport Security icon and double-click to open the plugin dialog:
![Configuration - Step 1:  Double-click HTTP Strict Transport Security icon under "Other"](assets/Configure1.png?raw=true)

**3.** Check the box to 'Enable HTTP Strict Transport Security':
![Configuration - Step 2:  Check the box to Enable HTTP Strict Transport Security](assets/Configure2.png?raw=true)

## Enable for single application using web.config
HSTS can be enabled on a per-application / per-website basis by ammending your configuration in web.config.

In web.config, locate the configuration/system.webServer node and add the following:
```xml
<hsts max-age="31536000" includeSubDomains="true" enabled="true" insecureRedirect="true" />
```

If your web.config file does not already have the system.webServer node, the following is a snippet of how the configuration should look when HSTS is enabled:

```xml
<configuration>
    <system.webServer>
        <hsts max-age="31536000" includeSubDomains="true" enabled="true" insecureRedirect="true" />
    </system.webServer>
</configuration>
```
