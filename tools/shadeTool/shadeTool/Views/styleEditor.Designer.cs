namespace shadeTool.Views
{
    partial class styleEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(styleEditor));
            this.ColorSwatch = new System.Windows.Forms.Panel();
            this.SetColourButton = new System.Windows.Forms.Button();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.concaveButton = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.styleSelector = new System.Windows.Forms.ComboBox();
            this.sceneModelBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.addStyleButton = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sceneModelBindingSource)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // ColorSwatch
            // 
            this.ColorSwatch.Location = new System.Drawing.Point(6, 19);
            this.ColorSwatch.Name = "ColorSwatch";
            this.ColorSwatch.Size = new System.Drawing.Size(122, 125);
            this.ColorSwatch.TabIndex = 0;
            this.ColorSwatch.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // SetColourButton
            // 
            this.SetColourButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.SetColourButton.Location = new System.Drawing.Point(6, 150);
            this.SetColourButton.Name = "SetColourButton";
            this.SetColourButton.Size = new System.Drawing.Size(122, 55);
            this.SetColourButton.TabIndex = 1;
            this.SetColourButton.Text = "Set UI Colour";
            this.SetColourButton.UseVisualStyleBackColor = true;
            this.SetColourButton.Click += new System.EventHandler(this.SetColourButton_Click);
            // 
            // radioButton1
            // 
            this.radioButton1.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioButton1.AutoSize = true;
            this.radioButton1.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.radioButton1.Image = ((System.Drawing.Image)(resources.GetObject("radioButton1.Image")));
            this.radioButton1.Location = new System.Drawing.Point(7, 115);
            this.radioButton1.MinimumSize = new System.Drawing.Size(130, 90);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(130, 90);
            this.radioButton1.TabIndex = 2;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "convex";
            this.radioButton1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.radioButton1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // concaveButton
            // 
            this.concaveButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.concaveButton.AutoSize = true;
            this.concaveButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.concaveButton.Image = ((System.Drawing.Image)(resources.GetObject("concaveButton.Image")));
            this.concaveButton.Location = new System.Drawing.Point(7, 19);
            this.concaveButton.MinimumSize = new System.Drawing.Size(130, 90);
            this.concaveButton.Name = "concaveButton";
            this.concaveButton.Size = new System.Drawing.Size(130, 90);
            this.concaveButton.TabIndex = 3;
            this.concaveButton.TabStop = true;
            this.concaveButton.Text = "concave";
            this.concaveButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.concaveButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.concaveButton.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioButton1);
            this.groupBox1.Controls.Add(this.concaveButton);
            this.groupBox1.Location = new System.Drawing.Point(155, 76);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(143, 213);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Shape";
            // 
            // styleSelector
            // 
            this.styleSelector.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.styleSelector.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.styleSelector.FormattingEnabled = true;
            this.styleSelector.Location = new System.Drawing.Point(6, 19);
            this.styleSelector.Name = "styleSelector";
            this.styleSelector.Size = new System.Drawing.Size(230, 21);
            this.styleSelector.TabIndex = 7;
            this.styleSelector.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.styleSelector_DrawItem);
            this.styleSelector.SelectedValueChanged += new System.EventHandler(this.styleSelector_SelectedValueChanged);
            this.styleSelector.TextChanged += new System.EventHandler(this.styleSelector_TextChanged);
            this.styleSelector.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.styleSelector_KeyPress);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.ColorSwatch);
            this.groupBox2.Controls.Add(this.SetColourButton);
            this.groupBox2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.groupBox2.Location = new System.Drawing.Point(12, 76);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(137, 213);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Editor Display";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.addStyleButton);
            this.groupBox3.Controls.Add(this.styleSelector);
            this.groupBox3.Location = new System.Drawing.Point(12, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(286, 58);
            this.groupBox3.TabIndex = 9;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Selected Style";
            // 
            // addStyleButton
            // 
            this.addStyleButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.addStyleButton.Location = new System.Drawing.Point(242, 19);
            this.addStyleButton.Name = "addStyleButton";
            this.addStyleButton.Size = new System.Drawing.Size(38, 21);
            this.addStyleButton.TabIndex = 2;
            this.addStyleButton.Text = "New";
            this.addStyleButton.UseVisualStyleBackColor = true;
            this.addStyleButton.Visible = false;
            this.addStyleButton.Click += new System.EventHandler(this.addStyleButton_Click);
            // 
            // styleEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(304, 299);
            this.ControlBox = false;
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "styleEditor";
            this.Text = "styleEditor";
            this.TopMost = true;
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sceneModelBindingSource)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel ColorSwatch;
        private System.Windows.Forms.Button SetColourButton;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.RadioButton concaveButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox styleSelector;
        private System.Windows.Forms.BindingSource sceneModelBindingSource;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button addStyleButton;
    }
}