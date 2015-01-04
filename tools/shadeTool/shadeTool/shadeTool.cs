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

        public shadeTool()
        {
            model = new SceneModel();
            controller = new EditController();

            this.WindowState = FormWindowState.Maximized;
            InitializeComponent();
            this.createBrushEditor();

            styleEditor se = new styleEditor();
            se.setModel(model);
            se.MdiParent = this;
            se.WindowState = FormWindowState.Normal;
            se.Show();
    
        }

        private void createBrushEditor()
        {
   
            topdownBrushEditor tbe = new topdownBrushEditor();
            tbe.setModel(model);
            tbe.MdiParent = this;
            tbe.Show();
        }

        private void topDownBrushEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.createBrushEditor();
        }

        private void entityListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            entityList el = new entityList();
            el.MdiParent = this;
            el.Show();
        }
    }
}
