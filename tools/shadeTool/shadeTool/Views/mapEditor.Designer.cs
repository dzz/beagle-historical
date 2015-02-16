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
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.entityDefBox = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.toolStrip2 = new System.Windows.Forms.ToolStrip();
            this.addEntity = new System.Windows.Forms.ToolStripButton();
            this.delEntity = new System.Windows.Forms.ToolStripButton();
            this.label2 = new System.Windows.Forms.Label();
            this.entityName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.entityListing = new System.Windows.Forms.ListBox();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.toolStrip2.SuspendLayout();
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
            this.statusStrip1.Location = new System.Drawing.Point(0, 379);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(740, 22);
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
            this.renderModeDropDown,
            this.toolStripButton3});
            this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip1.Size = new System.Drawing.Size(740, 23);
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
            // toolStripButton3
            // 
            this.toolStripButton3.Checked = true;
            this.toolStripButton3.CheckOnClick = true;
            this.toolStripButton3.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(117, 17);
            this.toolStripButton3.Text = "[ entity editor toggle ]";
            this.toolStripButton3.CheckedChanged += new System.EventHandler(this.toolStripButton3_CheckedChanged);
            // 
            // entityDefBox
            // 
            this.entityDefBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.entityDefBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(32)))), ((int)(((byte)(48)))));
            this.entityDefBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.entityDefBox.Font = new System.Drawing.Font("Courier New", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.entityDefBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(192)))));
            this.entityDefBox.Location = new System.Drawing.Point(188, 0);
            this.entityDefBox.Multiline = true;
            this.entityDefBox.Name = "entityDefBox";
            this.entityDefBox.Size = new System.Drawing.Size(552, 194);
            this.entityDefBox.TabIndex = 4;
            this.entityDefBox.Text = "{}";
            this.entityDefBox.TextChanged += new System.EventHandler(this.entityDefBox_TextChanged);
            this.entityDefBox.MouseLeave += new System.EventHandler(this.entityDefBox_MouseLeave);
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.entityName);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.entityListing);
            this.panel1.Controls.Add(this.entityDefBox);
            this.panel1.Location = new System.Drawing.Point(0, 185);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(740, 194);
            this.panel1.TabIndex = 5;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.toolStrip2);
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(188, 30);
            this.panel2.TabIndex = 9;
            // 
            // toolStrip2
            // 
            this.toolStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addEntity,
            this.delEntity});
            this.toolStrip2.Location = new System.Drawing.Point(0, 0);
            this.toolStrip2.Name = "toolStrip2";
            this.toolStrip2.Size = new System.Drawing.Size(188, 25);
            this.toolStrip2.TabIndex = 0;
            this.toolStrip2.Text = "toolStrip2";
            // 
            // addEntity
            // 
            this.addEntity.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.addEntity.Image = ((System.Drawing.Image)(resources.GetObject("addEntity.Image")));
            this.addEntity.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addEntity.Name = "addEntity";
            this.addEntity.Size = new System.Drawing.Size(43, 22);
            this.addEntity.Text = "[ add ]";
            this.addEntity.Click += new System.EventHandler(this.addEntity_Click);
            // 
            // delEntity
            // 
            this.delEntity.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.delEntity.Image = ((System.Drawing.Image)(resources.GetObject("delEntity.Image")));
            this.delEntity.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.delEntity.Name = "delEntity";
            this.delEntity.Size = new System.Drawing.Size(36, 22);
            this.delEntity.Text = "[del ]";
            this.delEntity.Click += new System.EventHandler(this.delEntity_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(3, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "[ tile entities ]";
            // 
            // entityName
            // 
            this.entityName.Location = new System.Drawing.Point(45, 33);
            this.entityName.Name = "entityName";
            this.entityName.Size = new System.Drawing.Size(137, 20);
            this.entityName.TabIndex = 7;
            this.entityName.TextChanged += new System.EventHandler(this.entityName_TextChanged);
            this.entityName.KeyDown += new System.Windows.Forms.KeyEventHandler(this.entityName_KeyDown);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 36);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "name:";
            // 
            // entityListing
            // 
            this.entityListing.FormattingEnabled = true;
            this.entityListing.Location = new System.Drawing.Point(3, 81);
            this.entityListing.Name = "entityListing";
            this.entityListing.Size = new System.Drawing.Size(179, 108);
            this.entityListing.TabIndex = 5;
            this.entityListing.SelectedIndexChanged += new System.EventHandler(this.entityListing_SelectedIndexChanged);
            // 
            // mapEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(740, 401);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.anchorLabel);
            this.Controls.Add(this.zLabel);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "mapEditor";
            this.Text = "topdownBrushEditor";
            this.Load += new System.EventHandler(this.mapEditor_Load);
            this.SizeChanged += new System.EventHandler(this.mapEditor_SizeChanged);
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
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.toolStrip2.ResumeLayout(false);
            this.toolStrip2.PerformLayout();
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
        private System.Windows.Forms.TextBox entityDefBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox entityName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox entityListing;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.ToolStrip toolStrip2;
        private System.Windows.Forms.ToolStripButton addEntity;
        private System.Windows.Forms.ToolStripButton delEntity;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
    }
}