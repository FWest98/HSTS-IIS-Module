#define _WINSOCKAPI_
#include <httpserv.h>
#include <comdef.h>

// Create the module class.
class MyHttpModule : public CHttpModule
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
		if ( pHttpSite != NULL )
		{
			pHttpSite = NULL;
		}
		if ( pElemMaxAgeProp != NULL )
		{
			pElemMaxAgeProp->Release(); 
			pElemMaxAgeProp = NULL;
		}
		if ( pElemIncludeSubDomainsProp != NULL )
		{
			pElemIncludeSubDomainsProp->Release(); 
			pElemIncludeSubDomainsProp = NULL;
		}
		if ( pElemEnabledProp != NULL )
		{
			pElemEnabledProp->Release(); 
			pElemEnabledProp = NULL;
		}
		if (pElemPreloadProp != NULL)
		{
			pElemPreloadProp->Release();
			pElemPreloadProp = NULL;
		}
		if ( pParentElem != NULL )
		{
			pParentElem->Release(); 
			pParentElem = NULL;
		}
		if ( pMgr != NULL )
		{
			pMgr->Release(); 
			pMgr = NULL;
		}

		if( bstrConfigCommitPath != NULL )
		{
			SysFreeString( bstrConfigCommitPath );
		}
		if( bstrSiteName != NULL )
		{
			SysFreeString( bstrSiteName );
		}

		SysFreeString( bstrSectionName );
		SysFreeString( bstrPropertyMaxAgeName );
		SysFreeString( bstrPropertyIncludeSubDomainsName );
		SysFreeString( bstrPropertyEnabledName );
		SysFreeString(bstrPropertyPreloadName);

		CoUninitialize();
	}


public:

    // Process an RQ_BEGIN_REQUEST event.
    REQUEST_NOTIFICATION_STATUS
    OnBeginRequest(
        IN IHttpContext * pHttpContext,
        IN IHttpEventProvider * pProvider
    )
    {
        UNREFERENCED_PARAMETER( pProvider );

		pMgr                       = NULL;
		pParentElem                = NULL;
		pElemProps                 = NULL;
		pElemMaxAgeProp            = NULL;
		pElemIncludeSubDomainsProp = NULL;
		pElemEnabledProp           = NULL;
		pElemPreloadProp           = NULL;
		pHttpSite                  = NULL;
		bstrSiteName               = NULL;
		bstrConfigCommitPath       = NULL;

		hr                                = S_OK;
		bstrConfigCommitBase              = SysAllocString( L"MACHINE/WEBROOT/APPHOST/" );
		bstrSectionName                   = SysAllocString( L"system.webServer/hsts" );
		bstrPropertyMaxAgeName            = SysAllocString( L"max-age" );
		bstrPropertyIncludeSubDomainsName = SysAllocString( L"includeSubDomains" );
		bstrPropertyEnabledName           = SysAllocString( L"enabled" );
		bstrPropertyInsecureRedirectName  = SysAllocString( L"insecureRedirect" );
		bstrPropertyPreloadName           = SysAllocString( L"preload" );

		vtPropertyMaxAgeName.vt                 = VT_BSTR;
		vtPropertyMaxAgeName.bstrVal            = bstrPropertyMaxAgeName;
		vtPropertyIncludeSubDomainsName.vt      = VT_BSTR;
		vtPropertyIncludeSubDomainsName.bstrVal = bstrPropertyIncludeSubDomainsName;
		vtPropertyEnabledName.vt                = VT_BSTR;
		vtPropertyEnabledName.bstrVal           = bstrPropertyEnabledName;
		vtPropertyInsecureRedirectName.vt       = VT_BSTR;
		vtPropertyInsecureRedirectName.bstrVal  = bstrPropertyInsecureRedirectName;
		vtPropertyPreloadName.vt                = VT_BSTR;
		vtPropertyPreloadName.bstrVal           = bstrPropertyPreloadName;

		//Initialize
		hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
		if ( FAILED( hr ) )
		{
			// Set the error status.
			pProvider->SetErrorStatus( hr );
			// cleanup
			cleanup();
			// End additional processing.
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		//Get config commit path
		pHttpSite = pHttpContext->GetSite();
		if( &pHttpSite == NULL )
		{
			pProvider->SetErrorStatus( E_FAIL );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}
		bstrSiteName = SysAllocString(pHttpSite->GetSiteName());
		bstrConfigCommitPath = SysAllocStringLen(bstrConfigCommitBase, SysStringLen(bstrConfigCommitBase) + SysStringLen(bstrSiteName) + 1);
		if(bstrConfigCommitPath == NULL) {
			pProvider->SetErrorStatus( HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY ) );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}
		wcscat(bstrConfigCommitPath, bstrSiteName);

		// Create
		hr = CoCreateInstance( __uuidof( AppHostAdminManager ), NULL, 
				CLSCTX_INPROC_SERVER,
				__uuidof( IAppHostAdminManager ), (void**) &pMgr );
		if( FAILED( hr ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		// Retrieve a pointer to the response.
		IHttpResponse * pHttpResponse = pHttpContext->GetResponse();

		// Test for an error.
		if ( pHttpResponse == NULL )
		{
			pProvider->SetErrorStatus( E_FAIL );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		// Get the hsts section
		hr = pMgr->GetAdminSection( bstrSectionName, bstrConfigCommitPath, &pParentElem );
		if ( FAILED( hr ) || ( &pParentElem == NULL ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		// Get the properties
		hr = pParentElem->get_Properties( &pElemProps );
		if ( FAILED( hr ) || ( &pElemProps == NULL ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemProps->get_Item( vtPropertyEnabledName, &pElemEnabledProp );
		if ( FAILED( hr ) || ( pElemEnabledProp == NULL ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemEnabledProp->get_Value( &vtEnabledValue );
		if ( FAILED( hr ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		bool enabled = V_BOOL( &vtEnabledValue );
		if(!enabled) {
			cleanup();
			return RQ_NOTIFICATION_CONTINUE;
		}

		DWORD httpsSize = 0;
		PCSTR https = NULL;
		pHttpContext->GetServerVariable( "HTTPS", &https, &httpsSize);
		https = (PCSTR)pHttpContext->AllocateRequestMemory( httpsSize + 1 );
		pHttpContext->GetServerVariable( "HTTPS", &https, &httpsSize);

		if(strcmp(https, "off")==0) {

			hr = pElemProps->get_Item( vtPropertyInsecureRedirectName, &pElemInsecureRedirectProp );
			if ( FAILED( hr ) || ( pElemInsecureRedirectProp == NULL ) )
			{
				pProvider->SetErrorStatus( hr );
				cleanup();
				return RQ_NOTIFICATION_FINISH_REQUEST;
			}

			hr = pElemInsecureRedirectProp->get_Value( &vtInsecureRedirectValue );
			if ( FAILED( hr ) )
			{
				pProvider->SetErrorStatus( hr );
				cleanup();
				return RQ_NOTIFICATION_FINISH_REQUEST;
			}

			bool insecureRedirect = V_BOOL( &vtInsecureRedirectValue );
			if(!insecureRedirect) {
				cleanup();
		        return RQ_NOTIFICATION_CONTINUE;
			}

			DWORD httpUrlSize = 0;
			PCSTR httpUrl = NULL;
			pHttpContext->GetServerVariable( "HTTP_URL", &httpUrl, &httpUrlSize);
			httpUrl = (PCSTR)pHttpContext->AllocateRequestMemory( httpUrlSize + 1 );
			pHttpContext->GetServerVariable( "HTTP_URL", &httpUrl, &httpUrlSize);

			DWORD serverNameSize = 0;
			PCSTR serverName = NULL;
			pHttpContext->GetServerVariable( "SERVER_NAME", &serverName, &serverNameSize);
			serverName = (PCSTR)pHttpContext->AllocateRequestMemory( serverNameSize + 1 );
			pHttpContext->GetServerVariable( "SERVER_NAME", &serverName, &serverNameSize);

			char* url = (char*)pHttpContext->AllocateRequestMemory( 8 /*https://*/ + serverNameSize + httpUrlSize + 1 );
			strcpy(url, "https://");
			strcat(url, serverName);
			strcat(url, httpUrl);

			pHttpResponse->Redirect(url, true, false);
			return RQ_NOTIFICATION_FINISH_REQUEST;

		}

		hr = pElemProps->get_Item( vtPropertyMaxAgeName, &pElemMaxAgeProp );
		if ( FAILED( hr ) || ( pElemMaxAgeProp == NULL ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemMaxAgeProp->get_Value( &vtMaxAgeValue );
		if ( FAILED( hr ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemProps->get_Item(vtPropertyPreloadName, &pElemPreloadProp);
		if (FAILED(hr) || (pElemPreloadProp == NULL))
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

		hr = pElemProps->get_Item( vtPropertyIncludeSubDomainsName, &pElemIncludeSubDomainsProp );
		if ( FAILED( hr ) || ( pElemIncludeSubDomainsProp == NULL ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		hr = pElemIncludeSubDomainsProp->get_Value( &vtIncludeSubDomainsValue );
		if ( FAILED( hr ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		bool includeSubDomains = V_BOOL( &vtIncludeSubDomainsValue );
		bool preload = V_BOOL(&vtPreloadValue);
		UINT iMaxAge = V_UINT( &vtMaxAgeValue );
		char maxAge [11];
		itoa(iMaxAge, maxAge, 10);

		char hstsName[] = "Strict-Transport-Security";
		// 27 for "max-age=" and "; includeSubDomains", 9 for "; preload", 11 for max size of UINT, 1 to terminate
		char * hstsValue = (char*)pHttpContext->AllocateRequestMemory( 48 );
		strcpy(hstsValue, "max-age=");
		strcat(hstsValue, maxAge);
		if(includeSubDomains) {
			strcat(hstsValue, "; includeSubDomains");
		}
		if (preload) {
			strcat(hstsValue, "; preload");
		}
		hr = pHttpResponse->SetHeader( hstsName, hstsValue, (USHORT)strlen(hstsValue), FALSE);
		if ( FAILED( hr ) )
		{
			pProvider->SetErrorStatus( hr );
			cleanup();
			return RQ_NOTIFICATION_FINISH_REQUEST;
		}

		cleanup();
        return RQ_NOTIFICATION_CONTINUE;
    }

};

// Create the module's class factory.
class MyHttpModuleFactory : public IHttpModuleFactory
{
public:
    HRESULT
    GetHttpModule(
        OUT CHttpModule ** ppModule, 
        IN IModuleAllocator * pAllocator
    )
    {
        UNREFERENCED_PARAMETER( pAllocator );

        // Create a new instance.
        MyHttpModule * pModule = new MyHttpModule;

        // Test for an error.
        if (!pModule)
        {
            // Return an error if the factory cannot create the instance.
            return HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
        }
        else
        {
            // Return a pointer to the module.
            *ppModule = pModule;
            pModule = NULL;
            // Return a success status.
            return S_OK;
        }            
    }

    void Terminate()
    {
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
    UNREFERENCED_PARAMETER( dwServerVersion );
    UNREFERENCED_PARAMETER( pGlobalInfo );

    // Set the request notifications and exit.
    return pModuleInfo->SetRequestNotifications(
        // Specify the class factory.
        new MyHttpModuleFactory,
        // Specify the event notifications.
        RQ_BEGIN_REQUEST,
        // Specify the post-event notifications.
        0
    );
}
