namespace InHomeServer
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.BtnSearch = new System.Windows.Forms.Button();
            this.CmbDeviceName = new System.Windows.Forms.ComboBox();
            this.PgrProgress = new System.Windows.Forms.ProgressBar();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // BtnSearch
            // 
            this.BtnSearch.Location = new System.Drawing.Point(81, 12);
            this.BtnSearch.Name = "BtnSearch";
            this.BtnSearch.Size = new System.Drawing.Size(75, 23);
            this.BtnSearch.TabIndex = 0;
            this.BtnSearch.Text = "Search";
            this.BtnSearch.UseVisualStyleBackColor = true;
            this.BtnSearch.Click += new System.EventHandler(this.button1_Click);
            // 
            // CmbDeviceName
            // 
            this.CmbDeviceName.FormattingEnabled = true;
            this.CmbDeviceName.Location = new System.Drawing.Point(54, 41);
            this.CmbDeviceName.Name = "CmbDeviceName";
            this.CmbDeviceName.Size = new System.Drawing.Size(121, 21);
            this.CmbDeviceName.TabIndex = 1;
            // 
            // PgrProgress
            // 
            this.PgrProgress.Location = new System.Drawing.Point(12, 68);
            this.PgrProgress.Name = "PgrProgress";
            this.PgrProgress.Size = new System.Drawing.Size(260, 23);
            this.PgrProgress.TabIndex = 2;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(54, 157);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 3;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.PgrProgress);
            this.Controls.Add(this.CmbDeviceName);
            this.Controls.Add(this.BtnSearch);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button BtnSearch;
        private System.Windows.Forms.ComboBox CmbDeviceName;
        private System.Windows.Forms.ProgressBar PgrProgress;
        private System.Windows.Forms.Button button1;
    }
}

