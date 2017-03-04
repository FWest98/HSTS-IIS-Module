// Win32Project1.cpp : Defines the exported functions for the DLL application.
//

#define _WINSOCKAPI_
#include <httpserv.h>
#include <comdef.h>

// Create the module class.
class HttpIisModule : public CHttpModule
{
private:
	IAppHostAdminManager        * pMgr;
	IAppHostElement             * pParentElem;
	IAppHostPropertyCollection  * pElemProps;
	IAppHostProperty            * pElemMaxAgeProp;
	IAppHostProperty            * pElemIncludeSubDomainsProp;
	IAppHostProperty            * pElemEnabledProp;
	IAppHostProperty            * pElemInsecureRedirectProp;
	IAppHostProperty            * pElemPreloadProp;
	IHttpSite                   * pHttpSite;

	HRESULT hr;
	BSTR    bstrConfigCommitPath;
	BSTR    bstrConfigCommitBase;
	BSTR    bstrSiteName;
	BSTR    bstrSectionName;
	BSTR    bstrPropertyMaxAgeName;
	BSTR    bstrPropertyIncludeSubDomainsName;
	BSTR    bstrPropertyEnabledName;
	BSTR    bstrPropertyInsecureRedirectName;
	BSTR    bstrPropertyPreloadName;

	VARIANT vtPropertyMaxAgeName;
	VARIANT vtPropertyIncludeSubDomainsName;
	VARIANT vtPropertyEnabledName;
	VARIANT vtPropertyInsecureRedirectName;
	VARIANT vtPropertyPreloadName;
	VARIANT vtMaxAgeValue;
	VARIANT vtIncludeSubDomainsValue;
	VARIANT vtEnabledValue;
	VARIANT vtInsecureRedirectValue;
	VARIANT vtPreloadValue;

	void cleanup()
	{
		if (pHttpSite != nullptr)
		{
			pHttpSite = nullptr;
		}
		if (pElemMaxAgeProp != nullptr)
		{
			pElemMaxAgeProp->Release();
			pElemMaxAgeProp = nullptr;
		}
		if (pElemIncludeSubDomainsProp != nullptr)
		{
			pElemIncludeSubDomainsProp->Release();
			pElemIncludeSubDomainsProp = nullptr;
		}
		if (pElemEnabledProp != nullptr)
		{
			pElemEnabledProp->Release();
			pElemEnabledProp = nullptr;
		}
		if (pElemPreloadProp != nullptr)
		{
			pElemPreloadProp->Release();
			pElemPreloadProp = nullptr;
		}
		if (pParentElem != nullptr)
		{
			pParentElem->Release();
			pParentElem = nullptr;
		}
		if (pMgr != nullptr)
		{
			pMgr->Release();
			pMgr = nullptr;
		}

		if (bstrConfigCommitPath != nullptr)
		{
			SysFreeString(bstrConfigCommitPath);
		}
		if (bstrSiteName != nullptr)
		{
			SysFreeString(bstrSiteName);
		}

		SysFreeString(bstrConfigCommitBase);
		SysFreeString(bstrSectionName);
		SysFreeString(bstrPropertyMaxAgeName);
		SysFreeString(bstrPropertyIncludeSubDomainsName);
		SysFreeString(bstrPropertyEnabledName);
		SysFreeString(bstrPropertyInsecureRedirectName);
		SysFreeString(bstrPropertyPreloadName);

		CoUninitialize();
	}


public:

	// Process an RQ_BEGIN_REQUEST event.
	REQUEST_NOTIFICATION_STATUS
		OnBeginRequest(
			IN IHttpContext * pHttpContext,
			IN IHttpEventProvider * pProvider
		) override {
		UNREFERENCED_PARAMETER(pProvider);

		pMgr = nullptr;
		pParentElem = nullptr;
		pElemProps = nullptr;
		pElemMaxAgeProp = nullptr;
		pElemIncludeSubDomainsProp = nullptr;
		pElemEnabledProp = nullptr;
		pElemPreloadProp = nullptr;
		pHttpSite = nullptr;
		bstrSiteName = nullptr;
		bstrConfigCommitPath = nullptr;

		hr = S_OK;
		bstrConfigCommitBase = SysAllocString(L"MACHINE/WEBROOT/APPHOST/");
		bstrSectionName = SysAllocString(L"system.webServer/hsts");
		bstrPropertyMaxAgeName = SysAllocString(L"max-age");
		bstrPropertyIncludeSubDomainsName = SysAllocString(L"includeSubDomains");
		bstrPropertyEnabledName = SysAllocString(L"enabled");
		bstrPropertyInsecureRedirectName = SysAllocString(L"insecureRedirect");
		bstrPropertyPreloadName = SysAllocString(L"preload");

		vtPropertyMaxAgeName.vt = VT_BSTR;
		vtPropertyMaxAgeName.bstrVal = bstrPropertyMaxAgeName;
		vtPropertyIncludeSubDomainsName.vt = VT_BSTR;
		vtPropertyIncludeSubDomainsName.bstrVal = bstrPropertyIncludeSubDomainsName;
		vtPropertyEnabledName.vt = VT_BSTR;
		vtPropertyEnabledName.bstrVal = bstrPropertyEnabledName;
		vtPropertyInsecureRedirectName.vt = VT_BSTR;
		vtPropertyInsecureRedirectName.bstrVal = bstrPropertyInsecureRedirectName;
		vtPropertyPreloadName.vt = VT_BSTR;
		vtPropertyPreloadName.bstrVal = bstrPropertyPreloadName;

		//Initialize
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
		{
			// Set the error status.
			pProvider->SetErrorStatus(hr);
			// cleanup
			cleanup();
			// End additional processing.
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		//Get config commit path
		pHttpSite = pHttpContext->GetSite();
		if (&pHttpSite == nullptr)
		{
			pProvider->SetErrorStatus(E_FAIL);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}
		bstrSiteName = SysAllocString(pHttpSite->GetSiteName());
		bstrConfigCommitPath = SysAllocStringLen(bstrConfigCommitBase, SysStringLen(bstrConfigCommitBase) + SysStringLen(bstrSiteName) + 1);
		if (bstrConfigCommitPath == nullptr) {
			pProvider->SetErrorStatus(HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY));
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}
		wcscat(bstrConfigCommitPath, bstrSiteName);

		// Create
		hr = CoCreateInstance(__uuidof(AppHostAdminManager), nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IAppHostAdminManager), reinterpret_cast<void**>(&pMgr));
		if (FAILED(hr))
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		// Retrieve a pointer to the response.
		auto pHttpResponse = pHttpContext->GetResponse();

		// Test for an error.
		if (pHttpResponse == nullptr)
		{
			pProvider->SetErrorStatus(E_FAIL);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		// Get the hsts section
		hr = pMgr->GetAdminSection(bstrSectionName, bstrConfigCommitPath, &pParentElem);
		if (FAILED(hr) || &pParentElem == nullptr)
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		// Get the properties
		hr = pParentElem->get_Properties(&pElemProps);
		if (FAILED(hr) || &pElemProps == nullptr)
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemProps->get_Item(vtPropertyEnabledName, &pElemEnabledProp);
		if (FAILED(hr) || pElemEnabledProp == nullptr)
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemEnabledProp->get_Value(&vtEnabledValue);
		if (FAILED(hr))
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		bool enabled = V_BOOL(&vtEnabledValue);
		if (!enabled) {
			cleanup();
			return RQ_NOTIFICATION_CONTINUE;
		}

		DWORD httpsSize = 0;
		PCSTR https = nullptr;
		pHttpContext->GetServerVariable("HTTPS", &https, &httpsSize);
		https = static_cast<PCSTR>(pHttpContext->AllocateRequestMemory(httpsSize + 1));
		pHttpContext->GetServerVariable("HTTPS", &https, &httpsSize);

		if (strcmp(https, "off") == 0) {

			hr = pElemProps->get_Item(vtPropertyInsecureRedirectName, &pElemInsecureRedirectProp);
			if (FAILED(hr) || pElemInsecureRedirectProp == nullptr)
			{
				pProvider->SetErrorStatus(hr);
				cleanup();
				return RQ_NOTIFICATION_FINISH_REQUEST;
			}

			hr = pElemInsecureRedirectProp->get_Value(&vtInsecureRedirectValue);
			if (FAILED(hr))
			{
				pProvider->SetErrorStatus(hr);
				cleanup();
				return RQ_NOTIFICATION_FINISH_REQUEST;
			}

			bool insecureRedirect = V_BOOL(&vtInsecureRedirectValue);
			if (!insecureRedirect) {
				cleanup();
				return RQ_NOTIFICATION_CONTINUE;
			}

			DWORD httpUrlSize = 0;
			PCSTR httpUrl = nullptr;
			pHttpContext->GetServerVariable("HTTP_URL", &httpUrl, &httpUrlSize);
			httpUrl = static_cast<PCSTR>(pHttpContext->AllocateRequestMemory(httpUrlSize + 1));
			pHttpContext->GetServerVariable("HTTP_URL", &httpUrl, &httpUrlSize);

			DWORD serverNameSize = 0;
			PCSTR serverName = nullptr;
			pHttpContext->GetServerVariable("SERVER_NAME", &serverName, &serverNameSize);
			serverName = static_cast<PCSTR>(pHttpContext->AllocateRequestMemory(serverNameSize + 1));
			pHttpContext->GetServerVariable("SERVER_NAME", &serverName, &serverNameSize);

			auto url = static_cast<char*>(pHttpContext->AllocateRequestMemory(8 /*https://*/ + serverNameSize + httpUrlSize + 1));
			strcpy(url, "https://");
			strcat(url, serverName);
			strcat(url, httpUrl);

			pHttpResponse->Redirect(url, true, false);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemProps->get_Item(vtPropertyMaxAgeName, &pElemMaxAgeProp);
		if (FAILED(hr) || pElemMaxAgeProp == nullptr)
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemMaxAgeProp->get_Value(&vtMaxAgeValue);
		if (FAILED(hr))
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemProps->get_Item(vtPropertyPreloadName, &pElemPreloadProp);
		if (FAILED(hr) || pElemPreloadProp == nullptr)
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemPreloadProp->get_Value(&vtPreloadValue);
		if (FAILED(hr))
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemProps->get_Item(vtPropertyIncludeSubDomainsName, &pElemIncludeSubDomainsProp);
		if (FAILED(hr) || pElemIncludeSubDomainsProp == nullptr)
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemIncludeSubDomainsProp->get_Value(&vtIncludeSubDomainsValue);
		if (FAILED(hr))
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		bool includeSubDomains = V_BOOL(&vtIncludeSubDomainsValue);
		bool preload = V_BOOL(&vtPreloadValue);
		auto iMaxAge = V_UINT(&vtMaxAgeValue);
		char maxAge[11];
		_itoa(iMaxAge, maxAge, 10);

		char hstsName[] = "Strict-Transport-Security";
		// 27 for "max-age=" and "; includeSubDomains", 9 for "; preload", 11 for max size of UINT, 1 to terminate
		auto hstsValue = static_cast<char*>(pHttpContext->AllocateRequestMemory(48));
		strcpy(hstsValue, "max-age=");
		strcat(hstsValue, maxAge);
		if (includeSubDomains) {
			strcat(hstsValue, "; includeSubDomains");
		}
		if (preload) {
			strcat(hstsValue, "; preload");
		}
		hr = pHttpResponse->SetHeader(hstsName, hstsValue, static_cast<USHORT>(strlen(hstsValue)), FALSE);
		if (FAILED(hr))
		{
			pProvider->SetErrorStatus(hr);
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		cleanup();
		return RQ_NOTIFICATION_CONTINUE;
	}

};

// Create the module's class factory.
class HttpIisModuleFactory : public IHttpModuleFactory
{
public:
	HRESULT
		GetHttpModule(
			OUT CHttpModule ** ppModule,
			IN IModuleAllocator * pAllocator
		) override {
		UNREFERENCED_PARAMETER(pAllocator);

		// Create a new instance.
		auto pModule = new HttpIisModule;

		// Test for an error.
		if (!pModule)
		{
			// Return an error if the factory cannot create the instance.
			return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
		}
		else
		{
			// Return a pointer to the module.
			*ppModule = pModule;
			pModule = nullptr;
			// Return a success status.
			return S_OK;
		}
	}

	void Terminate() override {
		// Remove the class from memory.
		delete this;
	}
};

// Create the module's exported registration function.
HRESULT
__stdcall
RegisterModule(
	DWORD dwServerVersion,
	IHttpModuleRegistrationInfo * pModuleInfo,
	IHttpServer * pGlobalInfo
)
{
	UNREFERENCED_PARAMETER(dwServerVersion);
	UNREFERENCED_PARAMETER(pGlobalInfo);

	// Set the request notifications and exit.
	return pModuleInfo->SetRequestNotifications(
		// Specify the class factory.
		new HttpIisModuleFactory,
		// Specify the event notifications.
		RQ_BEGIN_REQUEST,
		// Specify the post-event notifications.
		0
	);
}
