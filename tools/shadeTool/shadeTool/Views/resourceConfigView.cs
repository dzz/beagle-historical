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
            this.unitSizeBox.TextChanged += new EventHandler(unitSizeBox_TextChanged);
        }

        void unitSizeBox_TextChanged(object sender, EventArgs e)
        {
            int new_size = 0;
            if (int.TryParse(unitSizeBox.Text, out new_size))
            {
                this.model.world_unit_size = new_size;
            }
        }

        protected override void synchRootModel(Models.SceneModel model)
        {
            this.setPathText();
            this.unitSizeBox.Text = this.model.world_unit_size.ToString();
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
