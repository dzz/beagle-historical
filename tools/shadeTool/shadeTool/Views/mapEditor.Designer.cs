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
            this.anchorLabel = new System.Windows.Forms.Label();
            this.zLabel = new System.Windows.Forms.Label();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.coordsLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.brushLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.drawModeDropDown = new System.Windows.Forms.ToolStripDropDownButton();
            this.drawModeFloorsButton = new System.Windows.Forms.ToolStripMenuItem();
            this.drawModeWallsButton = new System.Windows.Forms.ToolStripMenuItem();
            this.addBrushButton = new System.Windows.Forms.ToolStripButton();
            this.selectBrushButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.upLayerButton = new System.Windows.Forms.ToolStripButton();
            this.downLayerButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.renderModeDropDown = new System.Windows.Forms.ToolStripDropDownButton();
            this.renderModeOnion = new System.Windows.Forms.ToolStripMenuItem();
            this.renderModeTextureCurrent = new System.Windows.Forms.ToolStripMenuItem();
            this.renderModeTextureAll = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // anchorLabel
            // 
            this.anchorLabel.AutoSize = true;
            this.anchorLabel.BackColor = System.Drawing.Color.Transparent;
            this.anchorLabel.Font = new System.Drawing.Font("Consolas", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.anchorLabel.ForeColor = System.Drawing.Color.Red;
            this.anchorLabel.Location = new System.Drawing.Point(12, 23);
            this.anchorLabel.Name = "anchorLabel";
            this.anchorLabel.Size = new System.Drawing.Size(225, 19);
            this.anchorLabel.TabIndex = 3;
            this.anchorLabel.Text = "waiting for brush anchor";
            this.anchorLabel.Visible = false;
            // 
            // zLabel
            // 
            this.zLabel.AutoSize = true;
            this.zLabel.BackColor = System.Drawing.Color.Transparent;
            this.zLabel.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.zLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.zLabel.Location = new System.Drawing.Point(10, 42);
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
            this.statusStrip1.Location = new System.Drawing.Point(0, 955);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(862, 22);
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
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.drawModeDropDown,
            this.addBrushButton,
            this.selectBrushButton,
            this.toolStripSeparator1,
            this.toolStripLabel2,
            this.upLayerButton,
            this.downLayerButton,
            this.toolStripSeparator2,
            this.renderModeDropDown});
            this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip1.Size = new System.Drawing.Size(862, 23);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            this.toolStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.toolStrip1_ItemClicked);
            this.toolStrip1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.toolStrip1_KeyDown);
            // 
            // drawModeDropDown
            // 
            this.drawModeDropDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.drawModeDropDown.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.drawModeFloorsButton,
            this.drawModeWallsButton});
            this.drawModeDropDown.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.drawModeDropDown.Image = ((System.Drawing.Image)(resources.GetObject("drawModeDropDown.Image")));
            this.drawModeDropDown.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.drawModeDropDown.Name = "drawModeDropDown";
            this.drawModeDropDown.Size = new System.Drawing.Size(93, 17);
            this.drawModeDropDown.Text = "[draw mode]";
            this.drawModeDropDown.ToolTipText = "[draw mode]";
            // 
            // drawModeFloorsButton
            // 
            this.drawModeFloorsButton.Checked = true;
            this.drawModeFloorsButton.CheckOnClick = true;
            this.drawModeFloorsButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this.drawModeFloorsButton.Name = "drawModeFloorsButton";
            this.drawModeFloorsButton.Size = new System.Drawing.Size(106, 22);
            this.drawModeFloorsButton.Text = "floors";
            this.drawModeFloorsButton.CheckedChanged += new System.EventHandler(this.drawModeFloorsButton_CheckedChanged);
            // 
            // drawModeWallsButton
            // 
            this.drawModeWallsButton.CheckOnClick = true;
            this.drawModeWallsButton.Name = "drawModeWallsButton";
            this.drawModeWallsButton.Size = new System.Drawing.Size(106, 22);
            this.drawModeWallsButton.Text = "walls";
            this.drawModeWallsButton.CheckedChanged += new System.EventHandler(this.drawModeWallsButton_CheckedChanged);
            // 
            // addBrushButton
            // 
            this.addBrushButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.addBrushButton.CheckOnClick = true;
            this.addBrushButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.addBrushButton.Image = ((System.Drawing.Image)(resources.GetObject("addBrushButton.Image")));
            this.addBrushButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addBrushButton.Name = "addBrushButton";
            this.addBrushButton.Size = new System.Drawing.Size(29, 17);
            this.addBrushButton.Text = "add";
            this.addBrushButton.Click += new System.EventHandler(this.addBrush_Click);
            // 
            // selectBrushButton
            // 
            this.selectBrushButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.selectBrushButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.selectBrushButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.selectBrushButton.Name = "selectBrushButton";
            this.selectBrushButton.Size = new System.Drawing.Size(39, 17);
            this.selectBrushButton.Text = "select";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 23);
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(98, 13);
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
            this.upLayerButton.Size = new System.Drawing.Size(23, 17);
            this.upLayerButton.Text = "up";
            this.upLayerButton.Click += new System.EventHandler(this.upLayer_Click);
            // 
            // downLayerButton
            // 
            this.downLayerButton.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.downLayerButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.downLayerButton.Image = ((System.Drawing.Image)(resources.GetObject("downLayerButton.Image")));
            this.downLayerButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.downLayerButton.Name = "downLayerButton";
            this.downLayerButton.Size = new System.Drawing.Size(37, 17);
            this.downLayerButton.Text = "down";
            this.downLayerButton.Click += new System.EventHandler(this.downLayer_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 23);
            // 
            // renderModeDropDown
            // 
            this.renderModeDropDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.renderModeDropDown.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.renderModeOnion,
            this.renderModeTextureCurrent,
            this.renderModeTextureAll});
            this.renderModeDropDown.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.renderModeDropDown.Image = ((System.Drawing.Image)(resources.GetObject("renderModeDropDown.Image")));
            this.renderModeDropDown.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.renderModeDropDown.Name = "renderModeDropDown";
            this.renderModeDropDown.Size = new System.Drawing.Size(103, 17);
            this.renderModeDropDown.Text = "[render mode]";
            this.renderModeDropDown.ToolTipText = "[render mode]";
            // 
            // renderModeOnion
            // 
            this.renderModeOnion.CheckOnClick = true;
            this.renderModeOnion.Name = "renderModeOnion";
            this.renderModeOnion.Size = new System.Drawing.Size(194, 22);
            this.renderModeOnion.Text = "onion";
            this.renderModeOnion.CheckedChanged += new System.EventHandler(this.renderModeOnion_CheckedChanged);
            // 
            // renderModeTextureCurrent
            // 
            this.renderModeTextureCurrent.Checked = true;
            this.renderModeTextureCurrent.CheckOnClick = true;
            this.renderModeTextureCurrent.CheckState = System.Windows.Forms.CheckState.Checked;
            this.renderModeTextureCurrent.Name = "renderModeTextureCurrent";
            this.renderModeTextureCurrent.Size = new System.Drawing.Size(194, 22);
            this.renderModeTextureCurrent.Text = "texture current layer";
            this.renderModeTextureCurrent.CheckedChanged += new System.EventHandler(this.renderModeTextureCurrent_CheckedChanged);
            // 
            // renderModeTextureAll
            // 
            this.renderModeTextureAll.CheckOnClick = true;
            this.renderModeTextureAll.Name = "renderModeTextureAll";
            this.renderModeTextureAll.Size = new System.Drawing.Size(194, 22);
            this.renderModeTextureAll.Text = "texture all layers";
            this.renderModeTextureAll.CheckedChanged += new System.EventHandler(this.renderModeTextureAll_CheckedChanged);
            // 
            // mapEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(862, 977);
            this.Controls.Add(this.anchorLabel);
            this.Controls.Add(this.zLabel);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "mapEditor";
            this.Text = "topdownBrushEditor";
            this.Load += new System.EventHandler(this.mapEditor_Load);
            this.Click += new System.EventHandler(this.mapEditor_Click);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.mapEditor_Paint);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.mapEditor_KeyDown);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.mapEditor_KeyPress);
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
        private System.Windows.Forms.ToolStripButton addBrushButton;
        private System.Windows.Forms.ToolStripButton selectBrushButton;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel2;
        private System.Windows.Forms.ToolStripButton upLayerButton;
        private System.Windows.Forms.ToolStripButton downLayerButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.Label zLabel;
        private System.Windows.Forms.Label anchorLabel;
        private System.Windows.Forms.ToolStripDropDownButton drawModeDropDown;
        private System.Windows.Forms.ToolStripMenuItem drawModeFloorsButton;
        private System.Windows.Forms.ToolStripMenuItem drawModeWallsButton;
        private System.Windows.Forms.ToolStripDropDownButton renderModeDropDown;
        private System.Windows.Forms.ToolStripMenuItem renderModeOnion;
        private System.Windows.Forms.ToolStripMenuItem renderModeTextureCurrent;
        private System.Windows.Forms.ToolStripMenuItem renderModeTextureAll;
    }
}