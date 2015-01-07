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

namespace shadeTool
{
    public partial class shadeTool : Form
    {

        SceneModel model;
        EditController controller;

        private void openView( ModelControllerView mcv, Rectangle layout ) {

            mcv.setController(controller);
            mcv.setModel( model );
            mcv.MdiParent = this;
            mcv.StartPosition = FormStartPosition.Manual;

            mcv.Location = new Point(layout.X, layout.Y);

            if (layout.Width > 0)
                mcv.Size = new Size(layout.Width, layout.Height);

            mcv.Show();


           
        }

        public shadeTool()
        {
            model = new SceneModel();
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
    }
}
