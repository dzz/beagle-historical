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
            this.brushListing = new System.Windows.Forms.ListBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.flatButton = new System.Windows.Forms.RadioButton();
            this.eastButton = new System.Windows.Forms.RadioButton();
            this.westButton = new System.Windows.Forms.RadioButton();
            this.southButton = new System.Windows.Forms.RadioButton();
            this.northButton = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.floor = new System.Windows.Forms.CheckBox();
            this.eastWall = new System.Windows.Forms.CheckBox();
            this.westWall = new System.Windows.Forms.CheckBox();
            this.southWall = new System.Windows.Forms.CheckBox();
            this.northWall = new System.Windows.Forms.CheckBox();
            this.textBoxName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.toolStrip1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // synchStyleButton
            // 
            this.synchStyleButton.Location = new System.Drawing.Point(199, 337);
            this.synchStyleButton.Name = "synchStyleButton";
            this.synchStyleButton.Size = new System.Drawing.Size(106, 46);
            this.synchStyleButton.TabIndex = 16;
            this.synchStyleButton.Text = "[ use active ]";
            this.synchStyleButton.UseVisualStyleBackColor = true;
            this.synchStyleButton.Click += new System.EventHandler(this.synchStyleButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(191, 290);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(40, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "[ style ]";
            // 
            // styleDisplay
            // 
            this.styleDisplay.Location = new System.Drawing.Point(199, 310);
            this.styleDisplay.Name = "styleDisplay";
            this.styleDisplay.ReadOnly = true;
            this.styleDisplay.Size = new System.Drawing.Size(106, 20);
            this.styleDisplay.TabIndex = 0;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.Left;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.moveBrushUpButton,
            this.moveBrushDownButton,
            this.toolStripSeparator1,
            this.toolStripLabel2,
            this.toolStripButton3});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(59, 406);
            this.toolStrip1.TabIndex = 14;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(56, 13);
            this.toolStripLabel1.Text = "[ order ]";
            // 
            // moveBrushUpButton
            // 
            this.moveBrushUpButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.moveBrushUpButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.moveBrushUpButton.Image = ((System.Drawing.Image)(resources.GetObject("moveBrushUpButton.Image")));
            this.moveBrushUpButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.moveBrushUpButton.Name = "moveBrushUpButton";
            this.moveBrushUpButton.Size = new System.Drawing.Size(56, 17);
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
            this.moveBrushDownButton.Size = new System.Drawing.Size(56, 17);
            this.moveBrushDownButton.Text = "[front]";
            this.moveBrushDownButton.Click += new System.EventHandler(this.moveBrushDownButton_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(56, 6);
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(56, 13);
            this.toolStripLabel2.Text = "[ action ]";
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(56, 17);
            this.toolStripButton3.Text = "[del]";
            this.toolStripButton3.Click += new System.EventHandler(this.toolStripButton3_Click);
            // 
            // brushListing
            // 
            this.brushListing.Font = new System.Drawing.Font("Lucida Sans Unicode", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.brushListing.FormattingEnabled = true;
            this.brushListing.ItemHeight = 15;
            this.brushListing.Location = new System.Drawing.Point(62, -1);
            this.brushListing.Name = "brushListing";
            this.brushListing.Size = new System.Drawing.Size(123, 409);
            this.brushListing.TabIndex = 0;
            this.brushListing.SelectedIndexChanged += new System.EventHandler(this.brushListing_SelectedIndexChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.flatButton);
            this.groupBox2.Controls.Add(this.eastButton);
            this.groupBox2.Controls.Add(this.westButton);
            this.groupBox2.Controls.Add(this.southButton);
            this.groupBox2.Controls.Add(this.northButton);
            this.groupBox2.Location = new System.Drawing.Point(199, 52);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(106, 117);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "[ ramp ]";
            // 
            // flatButton
            // 
            this.flatButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.flatButton.AutoSize = true;
            this.flatButton.Checked = true;
            this.flatButton.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.flatButton.Location = new System.Drawing.Point(47, 55);
            this.flatButton.MinimumSize = new System.Drawing.Size(15, 15);
            this.flatButton.Name = "flatButton";
            this.flatButton.Size = new System.Drawing.Size(15, 15);
            this.flatButton.TabIndex = 4;
            this.flatButton.TabStop = true;
            this.flatButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.flatButton.UseVisualStyleBackColor = true;
            this.flatButton.CheckedChanged += new System.EventHandler(this.flatButton_CheckedChanged);
            // 
            // eastButton
            // 
            this.eastButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.eastButton.AutoSize = true;
            this.eastButton.Cursor = System.Windows.Forms.Cursors.PanEast;
            this.eastButton.Location = new System.Drawing.Point(68, 51);
            this.eastButton.MinimumSize = new System.Drawing.Size(15, 15);
            this.eastButton.Name = "eastButton";
            this.eastButton.Size = new System.Drawing.Size(24, 23);
            this.eastButton.TabIndex = 3;
            this.eastButton.Text = "E";
            this.eastButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.eastButton.UseVisualStyleBackColor = true;
            this.eastButton.CheckedChanged += new System.EventHandler(this.eastButton_CheckedChanged);
            // 
            // westButton
            // 
            this.westButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.westButton.AutoSize = true;
            this.westButton.Cursor = System.Windows.Forms.Cursors.PanWest;
            this.westButton.Location = new System.Drawing.Point(13, 51);
            this.westButton.MinimumSize = new System.Drawing.Size(15, 15);
            this.westButton.Name = "westButton";
            this.westButton.Size = new System.Drawing.Size(28, 23);
            this.westButton.TabIndex = 2;
            this.westButton.Text = "W";
            this.westButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.westButton.UseVisualStyleBackColor = true;
            this.westButton.CheckedChanged += new System.EventHandler(this.westButton_CheckedChanged);
            // 
            // southButton
            // 
            this.southButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.southButton.AutoSize = true;
            this.southButton.Cursor = System.Windows.Forms.Cursors.PanSouth;
            this.southButton.Location = new System.Drawing.Point(44, 76);
            this.southButton.MinimumSize = new System.Drawing.Size(15, 15);
            this.southButton.Name = "southButton";
            this.southButton.Size = new System.Drawing.Size(24, 23);
            this.southButton.TabIndex = 1;
            this.southButton.Text = "S";
            this.southButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.southButton.UseVisualStyleBackColor = true;
            this.southButton.CheckedChanged += new System.EventHandler(this.southButton_CheckedChanged);
            // 
            // northButton
            // 
            this.northButton.Appearance = System.Windows.Forms.Appearance.Button;
            this.northButton.AutoSize = true;
            this.northButton.Cursor = System.Windows.Forms.Cursors.PanNorth;
            this.northButton.Location = new System.Drawing.Point(43, 22);
            this.northButton.MinimumSize = new System.Drawing.Size(15, 15);
            this.northButton.Name = "northButton";
            this.northButton.Size = new System.Drawing.Size(25, 23);
            this.northButton.TabIndex = 0;
            this.northButton.Text = "N";
            this.northButton.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.northButton.UseVisualStyleBackColor = true;
            this.northButton.CheckedChanged += new System.EventHandler(this.northButton_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.floor);
            this.groupBox1.Controls.Add(this.eastWall);
            this.groupBox1.Controls.Add(this.westWall);
            this.groupBox1.Controls.Add(this.southWall);
            this.groupBox1.Controls.Add(this.northWall);
            this.groupBox1.Location = new System.Drawing.Point(315, 175);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(105, 111);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "[ surfaces ]";
            this.groupBox1.Visible = false;
            // 
            // floor
            // 
            this.floor.Appearance = System.Windows.Forms.Appearance.Button;
            this.floor.AutoSize = true;
            this.floor.Location = new System.Drawing.Point(27, 36);
            this.floor.MinimumSize = new System.Drawing.Size(50, 50);
            this.floor.Name = "floor";
            this.floor.Size = new System.Drawing.Size(50, 50);
            this.floor.TabIndex = 9;
            this.floor.Text = "[ floor ]";
            this.floor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.floor.UseVisualStyleBackColor = true;
            this.floor.CheckedChanged += new System.EventHandler(this.floor_CheckedChanged);
            // 
            // eastWall
            // 
            this.eastWall.Appearance = System.Windows.Forms.Appearance.Button;
            this.eastWall.AutoSize = true;
            this.eastWall.Location = new System.Drawing.Point(79, 36);
            this.eastWall.MinimumSize = new System.Drawing.Size(10, 50);
            this.eastWall.Name = "eastWall";
            this.eastWall.Size = new System.Drawing.Size(10, 50);
            this.eastWall.TabIndex = 8;
            this.eastWall.UseVisualStyleBackColor = true;
            this.eastWall.CheckedChanged += new System.EventHandler(this.eastWall_CheckedChanged);
            // 
            // westWall
            // 
            this.westWall.Appearance = System.Windows.Forms.Appearance.Button;
            this.westWall.AutoSize = true;
            this.westWall.Location = new System.Drawing.Point(15, 36);
            this.westWall.MinimumSize = new System.Drawing.Size(10, 50);
            this.westWall.Name = "westWall";
            this.westWall.Size = new System.Drawing.Size(10, 50);
            this.westWall.TabIndex = 7;
            this.westWall.UseVisualStyleBackColor = true;
            this.westWall.CheckedChanged += new System.EventHandler(this.westWall_CheckedChanged);
            // 
            // southWall
            // 
            this.southWall.Appearance = System.Windows.Forms.Appearance.Button;
            this.southWall.AutoSize = true;
            this.southWall.Location = new System.Drawing.Point(27, 88);
            this.southWall.MinimumSize = new System.Drawing.Size(50, 10);
            this.southWall.Name = "southWall";
            this.southWall.Size = new System.Drawing.Size(50, 10);
            this.southWall.TabIndex = 6;
            this.southWall.UseVisualStyleBackColor = true;
            this.southWall.CheckedChanged += new System.EventHandler(this.southWall_CheckedChanged);
            // 
            // northWall
            // 
            this.northWall.Appearance = System.Windows.Forms.Appearance.Button;
            this.northWall.AutoSize = true;
            this.northWall.Location = new System.Drawing.Point(27, 24);
            this.northWall.MinimumSize = new System.Drawing.Size(50, 10);
            this.northWall.Name = "northWall";
            this.northWall.Size = new System.Drawing.Size(50, 10);
            this.northWall.TabIndex = 5;
            this.northWall.UseVisualStyleBackColor = true;
            this.northWall.CheckedChanged += new System.EventHandler(this.northWall_CheckedChanged);
            // 
            // textBoxName
            // 
            this.textBoxName.Location = new System.Drawing.Point(199, 26);
            this.textBoxName.Name = "textBoxName";
            this.textBoxName.Size = new System.Drawing.Size(106, 20);
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
            this.ClientSize = new System.Drawing.Size(315, 406);
            this.ControlBox = false;
            this.Controls.Add(this.synchStyleButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.styleDisplay);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.brushListing);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textBoxName);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "sceneBrushEditor";
            this.Text = "[ brush list ]";
            this.Load += new System.EventHandler(this.BrushList_Load);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxName;
        private System.Windows.Forms.CheckBox northWall;
        private System.Windows.Forms.CheckBox southWall;
        private System.Windows.Forms.CheckBox westWall;
        private System.Windows.Forms.CheckBox eastWall;
        private System.Windows.Forms.CheckBox floor;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton flatButton;
        private System.Windows.Forms.RadioButton eastButton;
        private System.Windows.Forms.RadioButton westButton;
        private System.Windows.Forms.RadioButton southButton;
        private System.Windows.Forms.RadioButton northButton;
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

    }
}