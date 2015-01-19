namespace shadeTool.Views
{
    partial class sceneBrushEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(sceneBrushEditor));
            this.label4 = new System.Windows.Forms.Label();
            this.wallNone = new System.Windows.Forms.RadioButton();
            this.wallDown = new System.Windows.Forms.RadioButton();
            this.wallUp = new System.Windows.Forms.RadioButton();
            this.label3 = new System.Windows.Forms.Label();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.synchStyleButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.styleDisplay = new System.Windows.Forms.TextBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.moveBrushUpButton = new System.Windows.Forms.ToolStripButton();
            this.moveBrushDownButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.brushListing = new System.Windows.Forms.ListBox();
            this.textBoxName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 457);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(33, 13);
            this.label4.TabIndex = 22;
            this.label4.Text = "ramp:";
            // 
            // wallNone
            // 
            this.wallNone.Appearance = System.Windows.Forms.Appearance.Button;
            this.wallNone.AutoSize = true;
            this.wallNone.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.wallNone.Location = new System.Drawing.Point(100, 451);
            this.wallNone.MinimumSize = new System.Drawing.Size(50, 15);
            this.wallNone.Name = "wallNone";
            this.wallNone.Size = new System.Drawing.Size(50, 25);
            this.wallNone.TabIndex = 21;
            this.wallNone.TabStop = true;
            this.wallNone.Text = "none";
            this.wallNone.UseVisualStyleBackColor = true;
            this.wallNone.CheckedChanged += new System.EventHandler(this.wallNone_CheckedChanged);
            // 
            // wallDown
            // 
            this.wallDown.Appearance = System.Windows.Forms.Appearance.Button;
            this.wallDown.AutoSize = true;
            this.wallDown.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.wallDown.Location = new System.Drawing.Point(149, 451);
            this.wallDown.MinimumSize = new System.Drawing.Size(50, 15);
            this.wallDown.Name = "wallDown";
            this.wallDown.Size = new System.Drawing.Size(50, 25);
            this.wallDown.TabIndex = 20;
            this.wallDown.TabStop = true;
            this.wallDown.Text = "down";
            this.wallDown.UseVisualStyleBackColor = true;
            this.wallDown.CheckedChanged += new System.EventHandler(this.wallDown_CheckedChanged);
            // 
            // wallUp
            // 
            this.wallUp.Appearance = System.Windows.Forms.Appearance.Button;
            this.wallUp.AutoSize = true;
            this.wallUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.wallUp.Location = new System.Drawing.Point(51, 451);
            this.wallUp.MinimumSize = new System.Drawing.Size(50, 15);
            this.wallUp.Name = "wallUp";
            this.wallUp.Size = new System.Drawing.Size(50, 25);
            this.wallUp.TabIndex = 19;
            this.wallUp.TabStop = true;
            this.wallUp.Text = "up";
            this.wallUp.UseVisualStyleBackColor = true;
            this.wallUp.CheckedChanged += new System.EventHandler(this.wallUp_CheckedChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 404);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 13);
            this.label3.TabIndex = 18;
            this.label3.Text = "name:";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(219, 458);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(65, 17);
            this.checkBox1.TabIndex = 17;
            this.checkBox1.Text = "billboard";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // synchStyleButton
            // 
            this.synchStyleButton.Location = new System.Drawing.Point(219, 425);
            this.synchStyleButton.Name = "synchStyleButton";
            this.synchStyleButton.Size = new System.Drawing.Size(84, 27);
            this.synchStyleButton.TabIndex = 16;
            this.synchStyleButton.Text = "[ use active ]";
            this.synchStyleButton.UseVisualStyleBackColor = true;
            this.synchStyleButton.Click += new System.EventHandler(this.synchStyleButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 425);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "style:";
            // 
            // styleDisplay
            // 
            this.styleDisplay.Location = new System.Drawing.Point(49, 425);
            this.styleDisplay.Name = "styleDisplay";
            this.styleDisplay.ReadOnly = true;
            this.styleDisplay.Size = new System.Drawing.Size(164, 20);
            this.styleDisplay.TabIndex = 0;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.moveBrushUpButton,
            this.moveBrushDownButton,
            this.toolStripSeparator1,
            this.toolStripLabel2,
            this.toolStripButton3,
            this.toolStripButton1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(340, 25);
            this.toolStrip1.TabIndex = 14;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(54, 22);
            this.toolStripLabel1.Text = "[ order ]";
            // 
            // moveBrushUpButton
            // 
            this.moveBrushUpButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.moveBrushUpButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.moveBrushUpButton.Image = ((System.Drawing.Image)(resources.GetObject("moveBrushUpButton.Image")));
            this.moveBrushUpButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.moveBrushUpButton.Name = "moveBrushUpButton";
            this.moveBrushUpButton.Size = new System.Drawing.Size(41, 22);
            this.moveBrushUpButton.Text = "[back]";
            this.moveBrushUpButton.Click += new System.EventHandler(this.moveBrushUpButton_Click);
            // 
            // moveBrushDownButton
            // 
            this.moveBrushDownButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.moveBrushDownButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.moveBrushDownButton.Image = ((System.Drawing.Image)(resources.GetObject("moveBrushDownButton.Image")));
            this.moveBrushDownButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.moveBrushDownButton.Name = "moveBrushDownButton";
            this.moveBrushDownButton.Size = new System.Drawing.Size(43, 22);
            this.moveBrushDownButton.Text = "[front]";
            this.moveBrushDownButton.Click += new System.EventHandler(this.moveBrushDownButton_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(58, 22);
            this.toolStripLabel2.Text = "[ action ]";
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(33, 22);
            this.toolStripButton3.Text = "[del]";
            this.toolStripButton3.Click += new System.EventHandler(this.toolStripButton3_Click);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(46, 22);
            this.toolStripButton1.Text = "[del all]";
            this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // brushListing
            // 
            this.brushListing.Font = new System.Drawing.Font("Lucida Sans Unicode", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.brushListing.FormattingEnabled = true;
            this.brushListing.ItemHeight = 15;
            this.brushListing.Location = new System.Drawing.Point(0, 29);
            this.brushListing.Name = "brushListing";
            this.brushListing.Size = new System.Drawing.Size(315, 364);
            this.brushListing.TabIndex = 0;
            this.brushListing.SelectedIndexChanged += new System.EventHandler(this.brushListing_SelectedIndexChanged);
            // 
            // textBoxName
            // 
            this.textBoxName.Location = new System.Drawing.Point(49, 401);
            this.textBoxName.Name = "textBoxName";
            this.textBoxName.Size = new System.Drawing.Size(254, 20);
            this.textBoxName.TabIndex = 2;
            this.textBoxName.TextChanged += new System.EventHandler(this.textBoxName_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(191, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "[ name ]";
            // 
            // sceneBrushEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(340, 485);
            this.ControlBox = false;
            this.Controls.Add(this.label4);
            this.Controls.Add(this.wallNone);
            this.Controls.Add(this.wallDown);
            this.Controls.Add(this.wallUp);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.synchStyleButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.styleDisplay);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.brushListing);
            this.Controls.Add(this.textBoxName);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "sceneBrushEditor";
            this.Text = "[ brush list ]";
            this.Load += new System.EventHandler(this.BrushList_Load);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxName;
        private System.Windows.Forms.TextBox styleDisplay;
        private System.Windows.Forms.ListBox brushListing;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripButton moveBrushUpButton;
        private System.Windows.Forms.ToolStripButton moveBrushDownButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel2;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button synchStyleButton;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.RadioButton wallUp;
        private System.Windows.Forms.RadioButton wallDown;
        private System.Windows.Forms.RadioButton wallNone;
        private System.Windows.Forms.Label label4;

    }
}