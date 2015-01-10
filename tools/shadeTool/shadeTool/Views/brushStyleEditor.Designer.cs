namespace shadeTool.Views
{
    partial class brushStyleEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(brushStyleEditor));
            this.addStyleButton = new System.Windows.Forms.Button();
            this.styleSelector = new System.Windows.Forms.ComboBox();
            this.ColorSwatch = new System.Windows.Forms.Panel();
            this.SetColourButton = new System.Windows.Forms.Button();
            this.sceneModelBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.Colours = new System.Windows.Forms.GroupBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.texLibraryButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            ((System.ComponentModel.ISupportInitialize)(this.sceneModelBindingSource)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.panel1.SuspendLayout();
            this.Colours.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // addStyleButton
            // 
            this.addStyleButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.addStyleButton.Location = new System.Drawing.Point(287, 6);
            this.addStyleButton.Name = "addStyleButton";
            this.addStyleButton.Size = new System.Drawing.Size(24, 21);
            this.addStyleButton.TabIndex = 2;
            this.addStyleButton.Text = "+";
            this.addStyleButton.UseVisualStyleBackColor = true;
            this.addStyleButton.Visible = false;
            this.addStyleButton.Click += new System.EventHandler(this.addStyleButton_Click);
            // 
            // styleSelector
            // 
            this.styleSelector.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.styleSelector.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.styleSelector.FormattingEnabled = true;
            this.styleSelector.Location = new System.Drawing.Point(6, 6);
            this.styleSelector.Name = "styleSelector";
            this.styleSelector.Size = new System.Drawing.Size(273, 21);
            this.styleSelector.TabIndex = 7;
            this.styleSelector.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.styleSelector_DrawItem);
            this.styleSelector.SelectedValueChanged += new System.EventHandler(this.styleSelector_SelectedValueChanged);
            this.styleSelector.TextChanged += new System.EventHandler(this.styleSelector_TextChanged);
            this.styleSelector.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.styleSelector_KeyPress);
            // 
            // ColorSwatch
            // 
            this.ColorSwatch.Location = new System.Drawing.Point(9, 45);
            this.ColorSwatch.Name = "ColorSwatch";
            this.ColorSwatch.Size = new System.Drawing.Size(128, 128);
            this.ColorSwatch.TabIndex = 0;
            this.ColorSwatch.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // SetColourButton
            // 
            this.SetColourButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.SetColourButton.Location = new System.Drawing.Point(6, 179);
            this.SetColourButton.Name = "SetColourButton";
            this.SetColourButton.Size = new System.Drawing.Size(135, 24);
            this.SetColourButton.TabIndex = 1;
            this.SetColourButton.Text = "select ...";
            this.SetColourButton.UseVisualStyleBackColor = true;
            this.SetColourButton.Click += new System.EventHandler(this.SetColourButton_Click);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(78, 19);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(50, 17);
            this.radioButton1.TabIndex = 11;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "scale";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(17, 19);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(55, 17);
            this.radioButton2.TabIndex = 12;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "repeat";
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.radioButton2);
            this.groupBox4.Controls.Add(this.radioButton1);
            this.groupBox4.Location = new System.Drawing.Point(4, 259);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(150, 48);
            this.groupBox4.TabIndex = 14;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "UV mode";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.comboBox1);
            this.groupBox6.Location = new System.Drawing.Point(158, 259);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(145, 48);
            this.groupBox6.TabIndex = 16;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "[ shader ]";
            // 
            // comboBox1
            // 
            this.comboBox1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(6, 19);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(130, 21);
            this.comboBox1.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panel1.Controls.Add(this.addStyleButton);
            this.panel1.Controls.Add(this.styleSelector);
            this.panel1.Location = new System.Drawing.Point(-2, -4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(323, 35);
            this.panel1.TabIndex = 17;
            // 
            // Colours
            // 
            this.Colours.Controls.Add(this.ColorSwatch);
            this.Colours.Controls.Add(this.SetColourButton);
            this.Colours.Location = new System.Drawing.Point(4, 40);
            this.Colours.Name = "Colours";
            this.Colours.Size = new System.Drawing.Size(150, 213);
            this.Colours.TabIndex = 18;
            this.Colours.TabStop = false;
            this.Colours.Text = "[ colour ]";
            // 
            // pictureBox1
            // 
            this.pictureBox1.InitialImage = ((System.Drawing.Image)(resources.GetObject("pictureBox1.InitialImage")));
            this.pictureBox1.Location = new System.Drawing.Point(9, 43);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(128, 128);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 13;
            this.pictureBox1.TabStop = false;
            // 
            // texLibraryButton
            // 
            this.texLibraryButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.texLibraryButton.Location = new System.Drawing.Point(9, 177);
            this.texLibraryButton.Name = "texLibraryButton";
            this.texLibraryButton.Size = new System.Drawing.Size(128, 26);
            this.texLibraryButton.TabIndex = 18;
            this.texLibraryButton.Text = "library ...";
            this.texLibraryButton.UseVisualStyleBackColor = true;
            this.texLibraryButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.comboBox2);
            this.groupBox1.Controls.Add(this.texLibraryButton);
            this.groupBox1.Controls.Add(this.pictureBox1);
            this.groupBox1.Location = new System.Drawing.Point(160, 40);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(152, 213);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "[ images ]";
            // 
            // comboBox2
            // 
            this.comboBox2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(9, 16);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(128, 21);
            this.comboBox2.TabIndex = 1;
            this.comboBox2.Text = "[ texture unit 1 ]";
            // 
            // brushStyleEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(315, 321);
            this.ControlBox = false;
            this.Controls.Add(this.Colours);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "brushStyleEditor";
            this.Text = "styleEditor";
            this.TopMost = true;
            ((System.ComponentModel.ISupportInitialize)(this.sceneModelBindingSource)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.Colours.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel ColorSwatch;
        private System.Windows.Forms.Button SetColourButton;
        private System.Windows.Forms.ComboBox styleSelector;
        private System.Windows.Forms.BindingSource sceneModelBindingSource;
        private System.Windows.Forms.Button addStyleButton;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox Colours;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button texLibraryButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox comboBox2;
    }
}