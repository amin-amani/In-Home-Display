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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.BtnSearch = new System.Windows.Forms.Button();
            this.CmbDeviceName = new System.Windows.Forms.ComboBox();
            this.PgrProgress = new System.Windows.Forms.ProgressBar();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.LabDeviceDate = new System.Windows.Forms.Label();
            this.LabVoltage = new System.Windows.Forms.Label();
            this.LabCurrent = new System.Windows.Forms.Label();
            this.LabEnergy = new System.Windows.Forms.Label();
            this.LabTemp = new System.Windows.Forms.Label();
            this.GrpMesure = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.BtnDownload = new System.Windows.Forms.Button();
            this.TimUpdateValues = new System.Windows.Forms.Timer(this.components);
            this.GrpMesure.SuspendLayout();
            this.SuspendLayout();
            // 
            // BtnSearch
            // 
            this.BtnSearch.Location = new System.Drawing.Point(21, 12);
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
            this.CmbDeviceName.Location = new System.Drawing.Point(143, 14);
            this.CmbDeviceName.Name = "CmbDeviceName";
            this.CmbDeviceName.Size = new System.Drawing.Size(121, 21);
            this.CmbDeviceName.TabIndex = 1;
            this.CmbDeviceName.Text = "192.168.1.100";
            // 
            // PgrProgress
            // 
            this.PgrProgress.Location = new System.Drawing.Point(12, 156);
            this.PgrProgress.Name = "PgrProgress";
            this.PgrProgress.Size = new System.Drawing.Size(260, 23);
            this.PgrProgress.TabIndex = 2;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(21, 99);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(116, 42);
            this.button1.TabIndex = 3;
            this.button1.Text = "Read Date And Time";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(21, 41);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(116, 42);
            this.button2.TabIndex = 4;
            this.button2.Text = "Sync Date And Time";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // LabDeviceDate
            // 
            this.LabDeviceDate.AutoSize = true;
            this.LabDeviceDate.Location = new System.Drawing.Point(6, 20);
            this.LabDeviceDate.Name = "LabDeviceDate";
            this.LabDeviceDate.Size = new System.Drawing.Size(70, 13);
            this.LabDeviceDate.TabIndex = 6;
            this.LabDeviceDate.Text = "Device Date:";
            // 
            // LabVoltage
            // 
            this.LabVoltage.AutoSize = true;
            this.LabVoltage.Location = new System.Drawing.Point(200, 41);
            this.LabVoltage.Name = "LabVoltage";
            this.LabVoltage.Size = new System.Drawing.Size(62, 13);
            this.LabVoltage.TabIndex = 7;
            this.LabVoltage.Text = "Voltage=0V";
            // 
            // LabCurrent
            // 
            this.LabCurrent.AutoSize = true;
            this.LabCurrent.Location = new System.Drawing.Point(200, 20);
            this.LabCurrent.Name = "LabCurrent";
            this.LabCurrent.Size = new System.Drawing.Size(60, 13);
            this.LabCurrent.TabIndex = 8;
            this.LabCurrent.Text = "Current=0A";
            // 
            // LabEnergy
            // 
            this.LabEnergy.AutoSize = true;
            this.LabEnergy.Location = new System.Drawing.Point(200, 62);
            this.LabEnergy.Name = "LabEnergy";
            this.LabEnergy.Size = new System.Drawing.Size(72, 13);
            this.LabEnergy.TabIndex = 9;
            this.LabEnergy.Text = "Energy=0 Wh";
            // 
            // LabTemp
            // 
            this.LabTemp.AutoSize = true;
            this.LabTemp.Location = new System.Drawing.Point(6, 41);
            this.LabTemp.Name = "LabTemp";
            this.LabTemp.Size = new System.Drawing.Size(56, 13);
            this.LabTemp.TabIndex = 10;
            this.LabTemp.Text = "Temp=0 C";
            // 
            // GrpMesure
            // 
            this.GrpMesure.Controls.Add(this.LabDeviceDate);
            this.GrpMesure.Controls.Add(this.LabEnergy);
            this.GrpMesure.Controls.Add(this.LabTemp);
            this.GrpMesure.Controls.Add(this.LabVoltage);
            this.GrpMesure.Controls.Add(this.LabCurrent);
            this.GrpMesure.Location = new System.Drawing.Point(295, 29);
            this.GrpMesure.Name = "GrpMesure";
            this.GrpMesure.Size = new System.Drawing.Size(377, 81);
            this.GrpMesure.TabIndex = 11;
            this.GrpMesure.TabStop = false;
            this.GrpMesure.Text = "Online Parameters";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(143, 41);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(116, 42);
            this.button3.TabIndex = 12;
            this.button3.Text = "Erase";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // BtnDownload
            // 
            this.BtnDownload.Location = new System.Drawing.Point(143, 99);
            this.BtnDownload.Name = "BtnDownload";
            this.BtnDownload.Size = new System.Drawing.Size(116, 42);
            this.BtnDownload.TabIndex = 13;
            this.BtnDownload.Text = "Download From Devivce";
            this.BtnDownload.UseVisualStyleBackColor = true;
            this.BtnDownload.Click += new System.EventHandler(this.BtnDownload_Click);
            // 
            // TimUpdateValues
            // 
            this.TimUpdateValues.Enabled = true;
            this.TimUpdateValues.Interval = 5000;
            this.TimUpdateValues.Tick += new System.EventHandler(this.TimUpdateValues_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(684, 231);
            this.Controls.Add(this.BtnDownload);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.GrpMesure);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.PgrProgress);
            this.Controls.Add(this.CmbDeviceName);
            this.Controls.Add(this.BtnSearch);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Energy Monitor V1.0";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.GrpMesure.ResumeLayout(false);
            this.GrpMesure.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button BtnSearch;
        private System.Windows.Forms.ComboBox CmbDeviceName;
        private System.Windows.Forms.ProgressBar PgrProgress;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label LabDeviceDate;
        private System.Windows.Forms.Label LabVoltage;
        private System.Windows.Forms.Label LabCurrent;
        private System.Windows.Forms.Label LabEnergy;
        private System.Windows.Forms.Label LabTemp;
        private System.Windows.Forms.GroupBox GrpMesure;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button BtnDownload;
        private System.Windows.Forms.Timer TimUpdateValues;
    }
}

