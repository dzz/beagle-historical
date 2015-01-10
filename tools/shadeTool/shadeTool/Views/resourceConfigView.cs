using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace shadeTool.Views
{
    public partial class resourceConfigView : ModelControllerView
    {
        public resourceConfigView()
        {
            InitializeComponent();
           

        }

        protected override void synchRootModel(Models.SceneModel model)
        {
            this.setPathText();
        }

        private void setPathText()
        {

            this.pathTextBox.Text = this.model.project_root;
        }
        private void browseRootButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.ShowNewFolderButton = true;
            if (fbd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.model.project_root = fbd.SelectedPath + "\\";
                this.synchRootModel(this.model);
            }
        }
    }
}
