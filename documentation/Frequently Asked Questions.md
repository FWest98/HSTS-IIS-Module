# Frequently Asked Questions

### Why build this module?
Whilst it is simple to add a custom header to an IIS site, there is no simple way to add the HSTS header in a way that is compliant with the draft specification ([RFC 6797](http://tools.ietf.org/html/rfc6797)). Specifically from [section 7.2](http://tools.ietf.org/html/rfc6797#section-7.2):

> An HSTS Host MUST NOT include the STS header field in HTTP responses conveyed over non-secure transport.

An additional driver for such a module is the seriousness of attack vectors such as [sslstrip](http://www.thoughtcrime.org/software/sslstrip/). It is hoped that simplicity of installation and configuration will avoid any excuse for not implementing the most effective defence against such attacks. 

### Why not just use an IIS custom header?
Short answer, you pretty much can and, in fact, this is often the answer on forums and wikis all over the Internet.

However, strictly speaking, this solution does not follow the [HSTS draft specification](http://tools.ietf.org/html/rfc6797#section-7.2) as the header will also be injected into non-secure responses. In reality, browsers ignore this header when sent over insecure channels, but this cannot be guaranteed. If a browser mistakenly accepts the header over an insecure channel, it's effectiveness is undermined. This is probably unlikely, but what is the point of a spec if it isn't followed.

Additionaly, as explained in "Why build this module?", I wanted to make configuring HSTS as simple as possible to encourage it's adoption.

### I have a criticism, an idea, an issue, a request, want to say hello.
Please feel free to contact me:
* Twitter: [@fwest98](https://twitter.com/fwest98)
* Issues: Please log issues [here at GitHub](https://github.com/FWest98/hsts-iis-module/issues)

### I am running this module in production/test/x86/x64/Server 2K8/Server 2k12/giraffe/anything.
That's not a question. But please tell me. I'd love to hear about it, especially if you are running it on a giraffe.

# General questions about HSTS

### Couldn't a man in the middle (MITM) simply strip the Strict-Transport-Security header?
Yes, for the first request the browser makes. The purpose of HSTS is to greatly reduce the attack surface, but doesn't completely eliminate the risk. This is the reason that the max-age is recommended to be so long (a year or two).

When the browser first makes a request to a domain, which returns the Strict-Transport-Security header, the browser records the domain in the HSTS list and sets the expiry to the 'max-age' seconds from now. This means that for the next year or two, that domain is protected from MITM attacks as the browser will refuse to communicate without SSL/TLS.

An example of this would be accessing your internet banking from home on your laptop. Your browser records your banks domain as secure. Now when you are out and about, accessing your internet banking from a coffee shop, you can rest assured that you are protected from a MITM.

Clearing the browser cache, history etc. does not clear the HSTS list. The list can be managed manually using in-built tools in the browser.

A way to make sure also the first request is 'safe' and not subject to a MITM-attack is to add your website to the HSTS Preload list. This is a list of domains that use HSTS, hardcoded in many browsers such as Firefox, Chrome, Safari, Internet Explorer and Edge. More information can be found [here](https://hstspreload.appspot.com/).

### What browsers support HSTS?
As this is an ever changing list, I will not provide a definitive list here. Generally speaking, most modern browsers support it, except Internet Explorer versions below 11. For information check out the [OWASP wiki page](https://www.owasp.org/index.php/HTTP_Strict_Transport_Security).
