namespace HSTS_IIS_Module.Manager
{
    partial class ConfigScreen
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.checkEnableHSTS = new System.Windows.Forms.CheckBox();
            this.checkInsecureRedirect = new System.Windows.Forms.CheckBox();
            this.groupHSTSSettings = new System.Windows.Forms.GroupBox();
            this.checkIncludeSubDomains = new System.Windows.Forms.CheckBox();
            this.checkPreload = new System.Windows.Forms.CheckBox();
            this.textMaxAge = new System.Windows.Forms.TextBox();
            this.labelMaxAge = new System.Windows.Forms.Label();
            this.groupHSTSSettings.SuspendLayout();
            this.SuspendLayout();
            // 
            // checkEnableHSTS
            // 
            this.checkEnableHSTS.AutoSize = true;
            this.checkEnableHSTS.Location = new System.Drawing.Point(3, 3);
            this.checkEnableHSTS.Name = "checkEnableHSTS";
            this.checkEnableHSTS.Size = new System.Drawing.Size(207, 17);
            this.checkEnableHSTS.TabIndex = 0;
            this.checkEnableHSTS.Text = "Enable HTTP Strict Transport Security";
            this.checkEnableHSTS.UseVisualStyleBackColor = true;
            this.checkEnableHSTS.CheckedChanged += new System.EventHandler(this.checkEnableHSTS_CheckedChanged);
            // 
            // groupHSTSSettings
            // 
            this.groupHSTSSettings.AutoSize = true;
            this.groupHSTSSettings.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupHSTSSettings.Controls.Add(this.checkIncludeSubDomains);
            this.groupHSTSSettings.Controls.Add(this.textMaxAge);
            this.groupHSTSSettings.Controls.Add(this.labelMaxAge);
            this.groupHSTSSettings.Controls.Add(this.checkInsecureRedirect);
            this.groupHSTSSettings.Controls.Add(this.checkPreload);
            this.groupHSTSSettings.Location = new System.Drawing.Point(3, 23);
            this.groupHSTSSettings.Name = "groupHSTSSettings";
            this.groupHSTSSettings.Size = new System.Drawing.Size(290, 108);
            this.groupHSTSSettings.TabIndex = 1;
            this.groupHSTSSettings.TabStop = false;
            this.groupHSTSSettings.Text = "HSTS Settings";
            // 
            // checkIncludeSubDomains
            // 
            this.checkIncludeSubDomains.AutoSize = true;
            this.checkIncludeSubDomains.Location = new System.Drawing.Point(9, 45);
            this.checkIncludeSubDomains.Name = "checkIncludeSubDomains";
            this.checkIncludeSubDomains.Size = new System.Drawing.Size(122, 17);
            this.checkIncludeSubDomains.TabIndex = 2;
            this.checkIncludeSubDomains.Text = "Include Subdomains";
            this.checkIncludeSubDomains.UseVisualStyleBackColor = true;
            // 
            // textMaxAge
            // 
            this.textMaxAge.Location = new System.Drawing.Point(63, 19);
            this.textMaxAge.Name = "textMaxAge";
            this.textMaxAge.Size = new System.Drawing.Size(221, 20);
            this.textMaxAge.TabIndex = 1;
            // 
            // labelMaxAge
            // 
            this.labelMaxAge.AutoSize = true;
            this.labelMaxAge.Location = new System.Drawing.Point(6, 22);
            this.labelMaxAge.Name = "labelMaxAge";
            this.labelMaxAge.Size = new System.Drawing.Size(51, 13);
            this.labelMaxAge.TabIndex = 0;
            this.labelMaxAge.Text = "Max age:";
            // 
            // checkInsecureRedirect
            // 
            this.checkInsecureRedirect.AutoSize = true;
            this.checkInsecureRedirect.Location = new System.Drawing.Point(9, 67);
            this.checkInsecureRedirect.Name = "checkInsecureRedirect";
            this.checkInsecureRedirect.Size = new System.Drawing.Size(122, 17);
            this.checkInsecureRedirect.TabIndex = 0;
            this.checkInsecureRedirect.Text = "Redirect Insecure Requests";
            this.checkInsecureRedirect.UseVisualStyleBackColor = true;
            //
            // checkPreload
            //
            this.checkPreload.AutoSize = true;
            this.checkPreload.Location = new System.Drawing.Point(9, 89);
            this.checkPreload.Name = "checkPreload";
            this.checkPreload.Size = new System.Drawing.Size(122, 17);
            this.checkPreload.TabIndex = 0;
            this.checkPreload.Text = "Allow Preload";
            this.checkPreload.UseVisualStyleBackColor = true;
            // 
            // ConfigScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Controls.Add(this.groupHSTSSettings);
            this.Controls.Add(this.checkEnableHSTS);
            this.Name = "ConfigScreen";
            this.Size = new System.Drawing.Size(296, 107);
            this.groupHSTSSettings.ResumeLayout(false);
            this.groupHSTSSettings.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkEnableHSTS;
        private System.Windows.Forms.CheckBox checkInsecureRedirect;
        private System.Windows.Forms.GroupBox groupHSTSSettings;
        private System.Windows.Forms.CheckBox checkIncludeSubDomains;
        private System.Windows.Forms.CheckBox checkPreload;
        private System.Windows.Forms.TextBox textMaxAge;
        private System.Windows.Forms.Label labelMaxAge;
    }
}
