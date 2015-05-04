using Microsoft.Web.Administration;
using System;
using System.Collections.Generic;
using System.Text;

namespace HSTS_IIS_Module.Manager
{
    class ConfigSection : ConfigurationSection
    {
        public const string MAX_AGE_PARAMETER_NAME = "max-age";
        public const string INCLUDE_SUB_DOMAINS_PARAMETER_NAME = "includeSubDomains";
        public const string ENABLED_PROPERTY_NAME = "enabled";
        public const string INSECURE_REDIRECT_NAME = "insecureRedirect";
        public const string PRELOAD_NAME = "preload";
        public const string CONFIG_PATH = "system.webServer/hsts";
        
        public bool Enabled
        {
            get
            {
                return (bool)base[ENABLED_PROPERTY_NAME];
            }
            set
            {
                base[ENABLED_PROPERTY_NAME] = value;
            }
        }

        public int MaxAge
        {
            get
            {
                return (int)base[MAX_AGE_PARAMETER_NAME];
            }
            set
            {
                base[MAX_AGE_PARAMETER_NAME] = value;
            }
        }

        public bool IncludeSubDomains
        {
            get
            {
                return (bool)base[INCLUDE_SUB_DOMAINS_PARAMETER_NAME];
            }
            set
            {
                base[INCLUDE_SUB_DOMAINS_PARAMETER_NAME] = value;
            }
        }

        public bool InsecureRedirect
        {
            get
            {
                return (bool)base[INSECURE_REDIRECT_NAME];
            }
            set
            {
                base[INSECURE_REDIRECT_NAME] = value;
            }
        }

        public bool Preload 
        {
            get 
            {
                return (bool)base[PRELOAD_NAME];
            }
            set 
            {
                base[PRELOAD_NAME] = value;
            }
        }
    }
}
