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
using System.IO;

namespace shadeTool.Views
{
    public partial class brushStyleEditor : ModelControllerView
    {
        String selectedStyle = "brush_default_style";

        protected override void synchRootModel(SceneModel model)
        {
            this.model.StylesChanged += new SceneModel.ModelChangedHandler(model_StylesChanged);
            this.synchStyles(this.model);
            List<string> shaders = new List<string>();

            var path = model.project_root + "shader\\";

            foreach (string filename in Directory.GetFiles(path, "*.json"))
            {
                shaders.Add(filename.Replace(path, ""));
            }

            foreach (string shader in shaders)
            {
        //        this.shaderSelector.Items.Add(shader);
            }
        }

        bool badDevon = false;

        void model_StylesChanged(SceneModel model)
        {
            if (!badDevon)
            {
                badDevon = true;
                this.synchStyles(model);
            }
            badDevon = false;
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
            this.controller.ActiveStyleKey = styleKey;
            this.selectedStyle = styleKey;
            this.ColorSwatch.Invalidate();

            try
            {
                Image texture = Image.FromFile(this.styleModel.texture);
                textureBox.Image = texture;
            }
            catch {
                textureBox.Image = null;
            }

            if (this.styleModel.uv_mode == BrushStyle.uv_mode_repeat)
                this.uvRepeatMode.Checked = true;
            else
                this.uvScaleMode.Checked = true;

           // this.shaderSelector.Text = this.styleModel.shader;
            this.rateBox.Text = this.styleModel.parallax.ToString();
            try
            {
                this.blendModeSelector.Text = this.model.layers[this.styleModel.layer];
            }
            catch { }

            this.layerSelector.Items.Clear();
            populateLayers();

            if (this.styleModel.parallax_object == false)
                this.radioButton1.Checked = true;
            else
                this.radioButton2.Checked = true;

            this.blendModeSelector.SelectedIndex = this.blendModeSelector.Items.IndexOf(this.blendModeSelector.Text);
        }

        private void populateLayers()
        {
           /* if (this.layerSelector.Items.Count > 0)
                return;*/

            int idx = 0;
            foreach (string layer in this.model.layers)
            {
                this.layerSelector.Items.Add(idx.ToString() + ":" + layer);
                ++idx;
            }
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

        public brushStyleEditor()
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
            string newStyle = this.styleSelector.Text;
            this.model.SetStyle(this.styleSelector.Text, new BrushStyle());

            this.selectStyle(newStyle);
            this.selectedStyle = newStyle;
            this.synchStyles(model);
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

        private void textureUnitSelector_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void browseTexture_Click(object sender, EventArgs e)
        {
            string img = shadeTool.browseLibraryFile("texture", "png", this.model);

            if (img != null ) {
                BrushStyle style = this.model.GetStyle(this.selectedStyle);
                style.texture = img;
                this.model.SetStyle(this.selectedStyle, style);
            }
        }

        private void uvRepeatMode_CheckedChanged(object sender, EventArgs e)
        {
            BrushStyle style = this.model.GetStyle(this.selectedStyle);

            if (style.uv_mode == BrushStyle.uv_mode_repeat)
                return;


            style.uv_mode = BrushStyle.uv_mode_repeat;
            this.model.SetStyle(this.selectedStyle, style);
        }

        private void uvScaleMode_CheckedChanged(object sender, EventArgs e)
        {
            
            BrushStyle style = this.model.GetStyle(this.selectedStyle);

            if (style.uv_mode == BrushStyle.uv_mode_scale)
                return;

            style.uv_mode = BrushStyle.uv_mode_scale;
            this.model.SetStyle(this.selectedStyle, style);
        }

        private void originLocalButton_CheckedChanged(object sender, EventArgs e)
        {
            this.styleModel.origin_mode = BrushStyle.origin_mode_local;
        }

        private void originWorldButton_CheckedChanged(object sender, EventArgs e)
        {
            this.styleModel.origin_mode = BrushStyle.origin_mode_global;
        }

        private void comboBox1_TextChanged(object sender, EventArgs e)
        {
           // this.styleModel.shader = shaderSelector.Text;
        }

        private void rateBox_TextChanged(object sender, EventArgs e)
        {
            double d = 0;
            if (double.TryParse(rateBox.Text, out d))
            {
                this.styleModel.parallax = d;
            }
        }

        private void layerSelector_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.styleModel.layer = this.layerSelector.SelectedIndex;
            this.blendModeSelector.SelectedIndex = this.blendModeSelector.Items.IndexOf(
                this.model.layers[this.styleModel.layer]);
        }


        private void blendModeSelector_SelectedIndexChanged(object sender, EventArgs e)
        {

            try
            {
                this.model.layers[this.layerSelector.SelectedIndex] = this.blendModeSelector.Text;

                this.layerSelector.Items.Clear();
                this.populateLayers();

                this.layerSelector.SelectedIndex = this.styleModel.layer;
            }
            catch { }
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            this.styleModel.parallax_object = radioButton2.Checked;
        }

    }
}
