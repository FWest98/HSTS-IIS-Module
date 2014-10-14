using Microsoft.Web.Management.Client.Win32;
using System;
using System.Collections.Generic;
using System.Text;

namespace HSTS_IIS_Module.Manager
{
    class ConfigModulePage : ModulePage
    {
        ConfigScreen configScreen;

        public ConfigModulePage()
        {
            configScreen = new ConfigScreen();
            Controls.Add(configScreen);
        }

        protected override void OnActivated(bool initialActivation)
        {
            base.OnActivated(initialActivation);
            if (initialActivation)
            {
                configScreen.Initialise(Connection.ConfigurationPath.SiteName);
            }
        }

        protected override void OnDeactivating(System.ComponentModel.CancelEventArgs e)
        {
            base.OnDeactivating(e);
            configScreen.Save();
        }
    }
}
