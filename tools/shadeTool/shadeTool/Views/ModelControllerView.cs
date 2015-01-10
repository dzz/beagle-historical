using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using shadeTool.Models;
using shadeTool.Controller;

namespace shadeTool.Views
{
    public partial class ModelControllerView : Form
    {
        protected SceneModel model;
        protected EditController controller;



        public void setController(EditController controller)
        {
            this.controller = controller;
            this.synchRootController(controller);
        }

        public void setModel(SceneModel model)
        {
            this.model = model;
            this.synchRootModel(model);
        }

        public ModelControllerView()
        {
            InitializeComponent();
        }

        protected virtual void synchRootModel(SceneModel model) { }
        protected virtual void synchRootController(EditController controller) { }

        private void ModelControllerView_LocationChanged(object sender, EventArgs e)
        {
            if (this is mapEditor)
                return;

        //    this.Location = new Point(this.MdiParent.Width-350, this.Location.Y);
        }
    }
}
