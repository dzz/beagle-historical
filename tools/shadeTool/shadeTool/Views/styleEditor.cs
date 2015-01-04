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
    public partial class styleEditor : ModelControllerView
    {
        String selectedStyle = "brush_default_style";

        SceneModel model;
        EditController controller;



        protected void synch(SceneModel model)
        {
            this.model.StylesChanged += new SceneModel.ModelChangedHandler(model_StylesChanged);
            this.synchStyles(this.model);
        }

        void model_StylesChanged(SceneModel model)
        {
            this.synchStyles(model);
        }
       
        public void synchStyles(SceneModel model)
        {
            this.styleSelector.Items.Clear();

            int count = 0;
            int selected = 0;
            foreach(String key in model.styles.Keys) {
                this.styleSelector.Items.Add(key);
          
                if (key == this.selectedStyle)
                {
                    selected = count;
                }
                count++;
            }

            this.styleSelector.SelectedIndex = selected;
        }



        private void selectStyle(string styleKey)
        {
            this.selectedStyle = styleKey;
            this.ColorSwatch.Invalidate();
        }

        public BrushStyle styleModel { get { 
                try {
                BrushStyle tmp = this.model.styles[this.selectedStyle]; 
                return tmp;
                }
                catch {
                    return new BrushStyle();
                }
            } 
        }


        public styleEditor()
        {
            InitializeComponent();

            
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(this.styleModel.UiColor);
        }

        private void SetColourButton_Click(object sender, EventArgs e)
        {
            ColorDialog cd = new ColorDialog();
            cd.ShowDialog();
            this.styleModel.UiColor = cd.Color;

            this.model.SetStyle(selectedStyle, this.styleModel);
            this.ColorSwatch.Invalidate();
        }

        private void styleSelector_SelectedValueChanged(object sender, EventArgs e)
        {
            this.setAddStyleVisibleState();
            this.selectStyle(this.styleSelector.Text);
        }

        private void setAddStyleVisibleState()
        {
            if (model.styles.ContainsKey(styleSelector.Text) == false)
            {
                this.addStyleButton.Visible = true;
            }
            else
            {
                this.addStyleButton.Visible = false;
            }
        }

        private void styleSelector_KeyPress(object sender, KeyPressEventArgs e)
        {
          
        }

        private void addStyleButton_Click(object sender, EventArgs e)
        {
            this.model.SetStyle(this.styleSelector.Text, new BrushStyle());

            this.selectedStyle = this.styleSelector.Text;
        }

        private void styleSelector_DrawItem(object sender, DrawItemEventArgs e)
        {
            e.DrawBackground();
            e.DrawFocusRectangle();

            string styleKey = this.styleSelector.Items[e.Index].ToString();

            BrushStyle styleModel = model.styles[styleKey];
            e.Graphics.DrawString(styleKey, e.Font, new
               SolidBrush(styleModel.UiColor), e.Bounds.Left, e.Bounds.Top);

            

        }

        private void styleSelector_TextChanged(object sender, EventArgs e)
        {
            this.setAddStyleVisibleState();
        }
    }
}
