using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using shadeTool.Models;
namespace shadeTool.Views
{
    public partial class sceneBrushEditor : ModelControllerView
    {
        public sceneBrushEditor()
        {
            InitializeComponent();

        }

        private void populateBrushListing()
        {
            brushListing.Items.Clear();

            int count = 0;
            int selected = 0;

            foreach (SceneBrush brush in model.brushes)
            {
                if (brush.z != this.controller.z_layer)
                    continue;

                this.brushListing.Items.Add(brush);

                if(brush == this.controller.ActiveBrush)
                    selected = count;
                count++;
            }

            if( count!=0) 
                this.brushListing.SelectedIndex = selected;
        }

        protected override void synchRootController(Controller.EditController controller)
        {

            this.controller.ActiveBrushChanged += new Controller.EditController.BrushHandler(controller_ActiveBrushChanged);
            this.controller.ZChanged += new Controller.EditController.ZHandler(controller_ZChanged);
        }

        void controller_ZChanged()
        {
            this.populateBrushListing();
        }

        void controller_ActiveBrushChanged(SceneBrush brush)
        {
            textBoxName.Text = brush.name;
            styleDisplay.Text = brush.styleName;

            this.northWall.Checked = brush.Walls[SceneBrush.NORTH_WALL];
            this.eastWall.Checked = brush.Walls[SceneBrush.EAST_WALL];
            this.southWall.Checked = brush.Walls[SceneBrush.SOUTH_WALL];
            this.westWall.Checked = brush.Walls[SceneBrush.WEST_WALL];

        }

        protected override void synchRootModel(Models.SceneModel model)
        {
            this.brushListing.Items.Clear();

            this.model.BrushesChanged += new SceneModel.ModelChangedHandler(model_BrushesChanged);
            this.populateBrushListing();
        }

        void model_BrushesChanged(SceneModel model)
        {
            this.populateBrushListing();
        }

        private void BrushList_Load(object sender, EventArgs e)
        {

        }

        private void synchStyleButton_Click(object sender, EventArgs e)
        {
            this.controller.ActiveBrush.styleName = this.controller.ActiveStyleKey;
            this.controller_ActiveBrushChanged( this.controller.ActiveBrush );
        }

        private void brushListing_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.controller.ActiveBrush = (SceneBrush) brushListing.SelectedItem;

        }

        private void textBoxName_TextChanged(object sender, EventArgs e)
        {
            if (this.controller.ActiveBrush != null )
            {
                this.controller.ActiveBrush.name = textBoxName.Text;

                this.populateBrushListing();
            }
        }

        private void floor_CheckedChanged(object sender, EventArgs e)
        {
            this.controller.ActiveBrush.Walls[SceneBrush.FLOOR] = this.floor.Checked;
        }

        private void northWall_CheckedChanged(object sender, EventArgs e)
        {
            this.controller.ActiveBrush.Walls[SceneBrush.NORTH_WALL] = this.northWall.Checked;
        }

        private void eastWall_CheckedChanged(object sender, EventArgs e)
        {
            this.controller.ActiveBrush.Walls[SceneBrush.EAST_WALL] = this.eastWall.Checked;
        }

        private void southWall_CheckedChanged(object sender, EventArgs e)
        {
            this.controller.ActiveBrush.Walls[SceneBrush.SOUTH_WALL] = this.southWall.Checked;
        }

        private void westWall_CheckedChanged(object sender, EventArgs e)
        {
            this.controller.ActiveBrush.Walls[SceneBrush.WEST_WALL] = this.westWall.Checked;
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            if (this.brushListing.SelectedItem != null)
            {
                this.model.DelBrush( (SceneBrush) this.brushListing.SelectedItem);
            }
        }

   
    }
}
