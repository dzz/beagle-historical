using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using shadeTool.Models;
using shadeTool.Views;
using shadeTool.Controller;
using System.IO;

namespace shadeTool
{
    public partial class shadeTool : Form
    {

        SceneModel model;
        EditController controller;


        int layout_top = 0;
        private void openView( ModelControllerView mcv, Rectangle layout ) {

            mcv.setController(controller);
            mcv.setModel( model );
            mcv.MdiParent = this;
            mcv.StartPosition = FormStartPosition.Manual;

            mcv.Location = new Point(layout.X, layout_top);

            if( (mcv is mapEditor)==false )
                layout_top += mcv.Height;

            if (layout.Width > 0)
                mcv.Size = new Size(layout.Width, layout.Height);

            mcv.Show();

            if ((mcv is mapEditor) == false )
            {
                mcv.LocationChanged += new EventHandler(mcv_LocationChanged);
            }


           
        }

        void mcv_LocationChanged(object sender, EventArgs e)
        {
            Form f = (Form)sender;
            f.Location = new Point( this.Width-350, f.Location.Y);
        }

        public shadeTool()
        {
            model = SceneModel.Load();
            controller = new EditController();

            this.WindowState = FormWindowState.Maximized;

           
            InitializeComponent();

            this.openView(new brushStyleEditor(), new Rectangle( 1400,0, 0,0) );
            this.openView(new mapEditor(), new Rectangle(0,0,1400,1000) );
            this.openView(new sceneBrushEditor(), new Rectangle( 1400,400,0,0) );
        
    
        }

    

        private void topDownBrushEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
         
        }

        private void entityListToolStripMenuItem_Click(object sender, EventArgs e)
        {
 
        }

        private void shadeTool_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.model.Save();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            try
            {
                this.model.Save();
            }
            catch (Exception x)
            {
                MessageBox.Show(x.Message);
            }
        }

        private void shadeTool_SizeChanged(object sender, EventArgs e)
        {
            foreach (Form child in this.MdiChildren)
            {
                if (child is mapEditor)
                {
                    child.Location = new Point(0, 0);
                    child.Size = new Size( this.ClientRectangle.Width - 350, this.ClientRectangle.Height);
                    
                }
                else
                {
                    child.Location = new Point(this.ClientRectangle.Width - 350, child.Location.Y);
                }
            }
        }

        public static string browseLibraryFile(string resourceType, string resourceExtension, SceneModel model)
        {
            OpenFileDialog ofd = new OpenFileDialog();

            string bootPath = Path.Combine(Path.GetDirectoryName(model.project_root), "texture");
            ofd.InitialDirectory = bootPath;

            ofd.Filter = resourceType + "|*." + resourceExtension;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                if (Path.GetDirectoryName(ofd.FileName) != bootPath)
                {
                    MessageBox.Show(
                        String.Format( "Please select a {0} from the project's {0} directory", resourceType) 
                        );
                    ofd.ShowDialog();
                }
                return ofd.FileName;
            }
            else
            {
                return null;
            }
        }


        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            resourceConfigView rcv = new resourceConfigView();

            rcv.setController(this.controller);
            rcv.setModel(this.model);

            rcv.ShowDialog();

        }
    }
}
