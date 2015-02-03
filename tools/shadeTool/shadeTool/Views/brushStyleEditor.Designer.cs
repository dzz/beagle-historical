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
            this.uvScaleMode = new System.Windows.Forms.RadioButton();
            this.uvRepeatMode = new System.Windows.Forms.RadioButton();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.shaderSelector = new System.Windows.Forms.ComboBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.Colours = new System.Windows.Forms.GroupBox();
            this.textureBox = new System.Windows.Forms.PictureBox();
            this.texLibraryButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.originLocalButton = new System.Windows.Forms.RadioButton();
            this.originWorldButton = new System.Windows.Forms.RadioButton();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.rateBox = new System.Windows.Forms.TextBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.layerSelector = new System.Windows.Forms.ListBox();
            this.layerGroupBox = new System.Windows.Forms.GroupBox();
            this.blendModeSelector = new System.Windows.Forms.ListBox();
            ((System.ComponentModel.ISupportInitialize)(this.sceneModelBindingSource)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.panel1.SuspendLayout();
            this.Colours.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.textureBox)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.layerGroupBox.SuspendLayout();
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
            this.ColorSwatch.Location = new System.Drawing.Point(13, 25);
            this.ColorSwatch.Name = "ColorSwatch";
            this.ColorSwatch.Size = new System.Drawing.Size(128, 128);
            this.ColorSwatch.TabIndex = 0;
            this.ColorSwatch.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // SetColourButton
            // 
            this.SetColourButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.SetColourButton.Location = new System.Drawing.Point(13, 161);
            this.SetColourButton.Name = "SetColourButton";
            this.SetColourButton.Size = new System.Drawing.Size(128, 24);
            this.SetColourButton.TabIndex = 1;
            this.SetColourButton.Text = "select ...";
            this.SetColourButton.UseVisualStyleBackColor = true;
            this.SetColourButton.Click += new System.EventHandler(this.SetColourButton_Click);
            // 
            // uvScaleMode
            // 
            this.uvScaleMode.AutoSize = true;
            this.uvScaleMode.Location = new System.Drawing.Point(78, 19);
            this.uvScaleMode.Name = "uvScaleMode";
            this.uvScaleMode.Size = new System.Drawing.Size(50, 17);
            this.uvScaleMode.TabIndex = 11;
            this.uvScaleMode.TabStop = true;
            this.uvScaleMode.Text = "scale";
            this.uvScaleMode.UseVisualStyleBackColor = true;
            this.uvScaleMode.CheckedChanged += new System.EventHandler(this.uvScaleMode_CheckedChanged);
            // 
            // uvRepeatMode
            // 
            this.uvRepeatMode.AutoSize = true;
            this.uvRepeatMode.Location = new System.Drawing.Point(17, 19);
            this.uvRepeatMode.Name = "uvRepeatMode";
            this.uvRepeatMode.Size = new System.Drawing.Size(55, 17);
            this.uvRepeatMode.TabIndex = 12;
            this.uvRepeatMode.TabStop = true;
            this.uvRepeatMode.Text = "repeat";
            this.uvRepeatMode.UseVisualStyleBackColor = true;
            this.uvRepeatMode.CheckedChanged += new System.EventHandler(this.uvRepeatMode_CheckedChanged);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.uvRepeatMode);
            this.groupBox4.Controls.Add(this.uvScaleMode);
            this.groupBox4.Location = new System.Drawing.Point(160, 246);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(150, 48);
            this.groupBox4.TabIndex = 14;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "[ uv mode ]";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.shaderSelector);
            this.groupBox6.Location = new System.Drawing.Point(4, 246);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(150, 48);
            this.groupBox6.TabIndex = 16;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "[ shader ]";
            // 
            // shaderSelector
            // 
            this.shaderSelector.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.shaderSelector.FormattingEnabled = true;
            this.shaderSelector.Location = new System.Drawing.Point(6, 19);
            this.shaderSelector.Name = "shaderSelector";
            this.shaderSelector.Size = new System.Drawing.Size(130, 21);
            this.shaderSelector.TabIndex = 0;
            this.shaderSelector.TextChanged += new System.EventHandler(this.comboBox1_TextChanged);
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
            this.Colours.Size = new System.Drawing.Size(150, 200);
            this.Colours.TabIndex = 18;
            this.Colours.TabStop = false;
            this.Colours.Text = "[ colour ]";
            // 
            // textureBox
            // 
            this.textureBox.InitialImage = ((System.Drawing.Image)(resources.GetObject("textureBox.InitialImage")));
            this.textureBox.Location = new System.Drawing.Point(6, 25);
            this.textureBox.Name = "textureBox";
            this.textureBox.Size = new System.Drawing.Size(128, 128);
            this.textureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.textureBox.TabIndex = 13;
            this.textureBox.TabStop = false;
            // 
            // texLibraryButton
            // 
            this.texLibraryButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.texLibraryButton.Location = new System.Drawing.Point(6, 159);
            this.texLibraryButton.Name = "texLibraryButton";
            this.texLibraryButton.Size = new System.Drawing.Size(128, 26);
            this.texLibraryButton.TabIndex = 18;
            this.texLibraryButton.Text = "library ...";
            this.texLibraryButton.UseVisualStyleBackColor = true;
            this.texLibraryButton.Click += new System.EventHandler(this.browseTexture_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.texLibraryButton);
            this.groupBox1.Controls.Add(this.textureBox);
            this.groupBox1.Location = new System.Drawing.Point(160, 40);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(152, 200);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "[ images ]";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.originLocalButton);
            this.groupBox2.Controls.Add(this.originWorldButton);
            this.groupBox2.Location = new System.Drawing.Point(159, 300);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(150, 48);
            this.groupBox2.TabIndex = 15;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "[ origin ]";
            // 
            // originLocalButton
            // 
            this.originLocalButton.AutoSize = true;
            this.originLocalButton.Location = new System.Drawing.Point(17, 19);
            this.originLocalButton.Name = "originLocalButton";
            this.originLocalButton.Size = new System.Drawing.Size(47, 17);
            this.originLocalButton.TabIndex = 12;
            this.originLocalButton.TabStop = true;
            this.originLocalButton.Text = "local";
            this.originLocalButton.UseVisualStyleBackColor = true;
            this.originLocalButton.CheckedChanged += new System.EventHandler(this.originLocalButton_CheckedChanged);
            // 
            // originWorldButton
            // 
            this.originWorldButton.AutoSize = true;
            this.originWorldButton.Location = new System.Drawing.Point(78, 19);
            this.originWorldButton.Name = "originWorldButton";
            this.originWorldButton.Size = new System.Drawing.Size(50, 17);
            this.originWorldButton.TabIndex = 11;
            this.originWorldButton.TabStop = true;
            this.originWorldButton.Text = "world";
            this.originWorldButton.UseVisualStyleBackColor = true;
            this.originWorldButton.CheckedChanged += new System.EventHandler(this.originWorldButton_CheckedChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.rateBox);
            this.groupBox3.Location = new System.Drawing.Point(4, 300);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(150, 48);
            this.groupBox3.TabIndex = 19;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "[ parallax ]";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "rate: (1 = none )";
            // 
            // rateBox
            // 
            this.rateBox.Location = new System.Drawing.Point(102, 19);
            this.rateBox.Name = "rateBox";
            this.rateBox.Size = new System.Drawing.Size(42, 20);
            this.rateBox.TabIndex = 0;
            this.rateBox.Text = "1.0";
            this.rateBox.TextChanged += new System.EventHandler(this.rateBox_TextChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.layerSelector);
            this.groupBox5.Location = new System.Drawing.Point(159, 354);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(149, 99);
            this.groupBox5.TabIndex = 20;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "[ render layer ]";
            // 
            // layerSelector
            // 
            this.layerSelector.FormattingEnabled = true;
            this.layerSelector.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7"});
            this.layerSelector.Location = new System.Drawing.Point(7, 20);
            this.layerSelector.Name = "layerSelector";
            this.layerSelector.Size = new System.Drawing.Size(136, 69);
            this.layerSelector.TabIndex = 0;
            this.layerSelector.SelectedIndexChanged += new System.EventHandler(this.layerSelector_SelectedIndexChanged);
            // 
            // layerGroupBox
            // 
            this.layerGroupBox.Controls.Add(this.blendModeSelector);
            this.layerGroupBox.Location = new System.Drawing.Point(4, 354);
            this.layerGroupBox.Name = "layerGroupBox";
            this.layerGroupBox.Size = new System.Drawing.Size(150, 99);
            this.layerGroupBox.TabIndex = 21;
            this.layerGroupBox.TabStop = false;
            // 
            // blendModeSelector
            // 
            this.blendModeSelector.FormattingEnabled = true;
            this.blendModeSelector.Items.AddRange(new object[] {
            "alpha",
            "multiply",
            "screen",
            "dodge"});
            this.blendModeSelector.Location = new System.Drawing.Point(6, 20);
            this.blendModeSelector.Name = "blendModeSelector";
            this.blendModeSelector.Size = new System.Drawing.Size(138, 69);
            this.blendModeSelector.TabIndex = 0;
            this.blendModeSelector.SelectedIndexChanged += new System.EventHandler(this.blendModeSelector_SelectedIndexChanged);
            // 
            // brushStyleEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(315, 458);
            this.ControlBox = false;
            this.Controls.Add(this.layerGroupBox);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
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
            ((System.ComponentModel.ISupportInitialize)(this.textureBox)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.layerGroupBox.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel ColorSwatch;
        private System.Windows.Forms.Button SetColourButton;
        private System.Windows.Forms.ComboBox styleSelector;
        private System.Windows.Forms.BindingSource sceneModelBindingSource;
        private System.Windows.Forms.Button addStyleButton;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.RadioButton uvRepeatMode;
        private System.Windows.Forms.RadioButton uvScaleMode;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.ComboBox shaderSelector;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox Colours;
        private System.Windows.Forms.PictureBox textureBox;
        private System.Windows.Forms.Button texLibraryButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton originLocalButton;
        private System.Windows.Forms.RadioButton originWorldButton;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox rateBox;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.ListBox layerSelector;
        private System.Windows.Forms.GroupBox layerGroupBox;
        private System.Windows.Forms.ListBox blendModeSelector;
    }
}