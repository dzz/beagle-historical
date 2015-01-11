namespace shadeTool.Views
{
    partial class ModelControllerView
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
            this.SuspendLayout();
            // 
            // ModelControllerView
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(292, 277);
            this.Name = "ModelControllerView";
            this.Text = "ModelControllerView";
            this.LocationChanged += new System.EventHandler(this.ModelControllerView_LocationChanged);
            this.MouseEnter += new System.EventHandler(this.ModelControllerView_MouseEnter);
            this.ResumeLayout(false);

        }

        #endregion
    }
}