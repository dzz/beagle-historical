namespace shadeTool.Views
{
    partial class resourceConfigView
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
            this.pathTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.browseRootButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // pathTextBox
            // 
            this.pathTextBox.Location = new System.Drawing.Point(72, 12);
            this.pathTextBox.Name = "pathTextBox";
            this.pathTextBox.Size = new System.Drawing.Size(452, 20);
            this.pathTextBox.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "project root:";
            // 
            // browseRootButton
            // 
            this.browseRootButton.Location = new System.Drawing.Point(530, 10);
            this.browseRootButton.Name = "browseRootButton";
            this.browseRootButton.Size = new System.Drawing.Size(63, 23);
            this.browseRootButton.TabIndex = 2;
            this.browseRootButton.Text = "set ...";
            this.browseRootButton.UseVisualStyleBackColor = true;
            this.browseRootButton.Click += new System.EventHandler(this.browseRootButton_Click);
            // 
            // resourceConfigView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(605, 46);
            this.ControlBox = false;
            this.Controls.Add(this.browseRootButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pathTextBox);
            this.Name = "resourceConfigView";
            this.Text = "[ project settings ]";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox pathTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button browseRootButton;
    }
}