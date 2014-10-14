using Microsoft.Web.Management.Client;
using Microsoft.Web.Management.Server;
using System;
using System.Collections.Generic;
using System.Text;

namespace HSTS_IIS_Module.Manager
{
    class ConfigModule : Module
    {
        protected override void Initialize(IServiceProvider serviceProvider, Microsoft.Web.Management.Server.ModuleInfo moduleInfo)
        {
            base.Initialize(serviceProvider, moduleInfo);

            IControlPanel controlPanel = (IControlPanel)GetService(typeof(IControlPanel));
            controlPanel.RegisterPage(new ModulePageInfo(this, typeof(ConfigModulePage),"HTTP Strict Transport Security", "Enable and configure HTTP Strict Transport Security."));
        }

        protected override bool IsPageEnabled(ModulePageInfo pageInfo)
        {
            Connection conn = (Connection)GetService(typeof(Connection));
            return conn.ConfigurationPath.PathType == ConfigurationPathType.Site;
        }
    }
}
