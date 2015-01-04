namespace shadeTool.Views
{
    partial class topdownBrushEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(topdownBrushEditor));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.coordsLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.brushLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.addBrushButton = new System.Windows.Forms.ToolStripButton();
            this.selectBrushButton = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.coordsLabel,
            this.brushLabel});
            this.statusStrip1.Location = new System.Drawing.Point(51, 433);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(827, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            this.statusStrip1.Click += new System.EventHandler(this.statusStrip1_Click);
            // 
            // coordsLabel
            // 
            this.coordsLabel.Name = "coordsLabel";
            this.coordsLabel.Size = new System.Drawing.Size(47, 17);
            this.coordsLabel.Text = "(Cursor)";
            // 
            // brushLabel
            // 
            this.brushLabel.Name = "brushLabel";
            this.brushLabel.Size = new System.Drawing.Size(42, 17);
            this.brushLabel.Text = "(Brush)";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(121, 13);
            this.toolStripLabel1.Text = "[ build ]";
            this.toolStripLabel1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // addBrushButton
            // 
            this.addBrushButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.addBrushButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.addBrushButton.Image = ((System.Drawing.Image)(resources.GetObject("addBrushButton.Image")));
            this.addBrushButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addBrushButton.Name = "addBrushButton";
            this.addBrushButton.Size = new System.Drawing.Size(48, 17);
            this.addBrushButton.Text = "Add";
            this.addBrushButton.Click += new System.EventHandler(this.addBrush_Click);
            // 
            // selectBrushButton
            // 
            this.selectBrushButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.selectBrushButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.selectBrushButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.selectBrushButton.Name = "selectBrushButton";
            this.selectBrushButton.Size = new System.Drawing.Size(121, 17);
            this.selectBrushButton.Text = "Select";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.Left;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.addBrushButton,
            this.selectBrushButton,
            this.toolStripSeparator1});
            this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.VerticalStackWithOverflow;
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip1.Size = new System.Drawing.Size(51, 455);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            this.toolStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.toolStrip1_ItemClicked);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(121, 6);
            // 
            // topdownBrushEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(878, 455);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.Name = "topdownBrushEditor";
            this.Text = "topdownBrushEditor";
            this.Click += new System.EventHandler(this.topdownBrushEditor_Click);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.topdownBrushEditor_Paint);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.topdownBrushEditor_MouseMove);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel coordsLabel;
        private System.Windows.Forms.ToolStripStatusLabel brushLabel;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripButton addBrushButton;
        private System.Windows.Forms.ToolStripButton selectBrushButton;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    }
}