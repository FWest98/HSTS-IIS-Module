using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Web.Administration;

namespace HSTS_IIS_Module.Manager
{
    public partial class ConfigScreen : UserControl
    {
        private string siteName;
        ServerManager serverManager;
        private ConfigSection configSection;

        public ConfigScreen()
        {
            InitializeComponent();
        }

        public void Initialise(string siteName)
        {
            this.siteName = siteName;
            serverManager = new ServerManager();

            ReloadConfigSection();

            checkEnableHSTS.Checked = configSection.Enabled;
            checkInsecureRedirect.Checked = configSection.InsecureRedirect;
            textMaxAge.Text = configSection.MaxAge.ToString();
            checkIncludeSubDomains.Checked = configSection.IncludeSubDomains;
            checkPreload.Checked = configSection.Preload;

            UpdateDisplay();
        }

        public void Save()
        {
            configSection.Enabled = checkEnableHSTS.Checked;
            configSection.InsecureRedirect = checkInsecureRedirect.Checked;

            long maxAge;
            configSection.MaxAge = long.TryParse(textMaxAge.Text, out maxAge) ? maxAge : 0;
            configSection.IncludeSubDomains = checkIncludeSubDomains.Checked;
            configSection.Preload = checkPreload.Checked;

            serverManager.CommitChanges();
        }

        private void ReloadConfigSection()
        {
            if (string.IsNullOrEmpty(siteName))
            {
                throw new Exception("Must 'Initialize' the component first.");
            }
            var config = serverManager.GetWebConfiguration(siteName);
            configSection = (ConfigSection)config.GetSection(ConfigSection.CONFIG_PATH, typeof(ConfigSection));
        }

        private void UpdateDisplay()
        {
            groupHSTSSettings.Enabled = checkEnableHSTS.Checked;
        }

        private void checkEnableHSTS_CheckedChanged(object sender, EventArgs e)
        {
            UpdateDisplay();
        }

    }
}
