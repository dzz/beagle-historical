namespace shadeTool.Views
{
    partial class mapEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mapEditor));
            this.zLabel = new System.Windows.Forms.Label();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.coordsLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.brushLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.addBrushButton = new System.Windows.Forms.ToolStripButton();
            this.drawModeSelector = new System.Windows.Forms.ToolStripComboBox();
            this.selectBrushButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.upLayerButton = new System.Windows.Forms.ToolStripButton();
            this.downLayerButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
            this.previewModeSelector = new System.Windows.Forms.ToolStripComboBox();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // zLabel
            // 
            this.zLabel.AutoSize = true;
            this.zLabel.BackColor = System.Drawing.Color.Transparent;
            this.zLabel.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.zLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.zLabel.Location = new System.Drawing.Point(142, 9);
            this.zLabel.Name = "zLabel";
            this.zLabel.Size = new System.Drawing.Size(135, 32);
            this.zLabel.TabIndex = 2;
            this.zLabel.Text = "layer: 0";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.coordsLabel,
            this.brushLabel});
            this.statusStrip1.Location = new System.Drawing.Point(114, 955);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(628, 22);
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
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.Left;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.addBrushButton,
            this.drawModeSelector,
            this.selectBrushButton,
            this.toolStripSeparator1,
            this.toolStripLabel2,
            this.upLayerButton,
            this.downLayerButton,
            this.toolStripSeparator2,
            this.toolStripLabel3,
            this.previewModeSelector});
            this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.VerticalStackWithOverflow;
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip1.Size = new System.Drawing.Size(114, 977);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            this.toolStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.toolStrip1_ItemClicked);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(111, 13);
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
            this.addBrushButton.Size = new System.Drawing.Size(111, 17);
            this.addBrushButton.Text = "Add";
            this.addBrushButton.Click += new System.EventHandler(this.addBrush_Click);
            // 
            // drawModeSelector
            // 
            this.drawModeSelector.Items.AddRange(new object[] {
            "floors",
            "walls"});
            this.drawModeSelector.Name = "drawModeSelector";
            this.drawModeSelector.Size = new System.Drawing.Size(109, 21);
            this.drawModeSelector.Text = "[ drawing mode ]";
            this.drawModeSelector.TextChanged += new System.EventHandler(this.drawModeSelector_TextChanged);
            // 
            // selectBrushButton
            // 
            this.selectBrushButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.selectBrushButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.selectBrushButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.selectBrushButton.Name = "selectBrushButton";
            this.selectBrushButton.Size = new System.Drawing.Size(111, 17);
            this.selectBrushButton.Text = "Select";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(111, 6);
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(111, 13);
            this.toolStripLabel2.Text = "[ cursor height ]";
            this.toolStripLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // upLayerButton
            // 
            this.upLayerButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.upLayerButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.upLayerButton.Image = ((System.Drawing.Image)(resources.GetObject("upLayerButton.Image")));
            this.upLayerButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.upLayerButton.Name = "upLayerButton";
            this.upLayerButton.Size = new System.Drawing.Size(111, 17);
            this.upLayerButton.Text = "Up Level";
            this.upLayerButton.Click += new System.EventHandler(this.upLayer_Click);
            // 
            // downLayerButton
            // 
            this.downLayerButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.downLayerButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.downLayerButton.Image = ((System.Drawing.Image)(resources.GetObject("downLayerButton.Image")));
            this.downLayerButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.downLayerButton.Name = "downLayerButton";
            this.downLayerButton.Size = new System.Drawing.Size(111, 17);
            this.downLayerButton.Text = "Down Level";
            this.downLayerButton.Click += new System.EventHandler(this.downLayer_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(111, 6);
            // 
            // toolStripLabel3
            // 
            this.toolStripLabel3.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel3.Name = "toolStripLabel3";
            this.toolStripLabel3.Size = new System.Drawing.Size(111, 13);
            this.toolStripLabel3.Text = "[ preview display ]";
            this.toolStripLabel3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // previewModeSelector
            // 
            this.previewModeSelector.Items.AddRange(new object[] {
            "onion",
            "texture"});
            this.previewModeSelector.Name = "previewModeSelector";
            this.previewModeSelector.Size = new System.Drawing.Size(109, 21);
            this.previewModeSelector.Text = "onion";
            this.previewModeSelector.TextChanged += new System.EventHandler(this.previewModeSelector_TextChanged);
            // 
            // mapEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(742, 977);
            this.Controls.Add(this.zLabel);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.Name = "mapEditor";
            this.Text = "topdownBrushEditor";
            this.Load += new System.EventHandler(this.mapEditor_Load);
            this.Click += new System.EventHandler(this.mapEditor_Click);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.mapEditor_Paint);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.mapEditor_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.mapEditor_MouseMove);
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
        private System.Windows.Forms.ToolStripLabel toolStripLabel2;
        private System.Windows.Forms.ToolStripButton upLayerButton;
        private System.Windows.Forms.ToolStripButton downLayerButton;
        private System.Windows.Forms.ToolStripComboBox drawModeSelector;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripLabel toolStripLabel3;
        private System.Windows.Forms.ToolStripComboBox previewModeSelector;
        private System.Windows.Forms.Label zLabel;
    }
}