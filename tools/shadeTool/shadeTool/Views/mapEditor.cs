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

using System.Drawing.Drawing2D;

namespace shadeTool.Views
{
    public partial class mapEditor : ModelControllerView
    {
        
        int unit_size   = 32;
        int half_unit   = 16;
        int camera_x    = 0;
        int camera_y    = 0;
        int camera_z    = 0;
        int cursor_x    = 0;
        int cursor_y    = 0;

        int state = 0;

        const int STATE_NONE = 0;
        const int STATE_BRUSH_DEFINE_A = 1;
        const int STATE_BRUSH_DEFINE_B = 2;

        Image testimg;

        public mapEditor()
        {
            InitializeComponent();

          //  this.WindowState = FormWindowState.Maximized;
            this.DoubleBuffered = true;

      
        }
        TextureBrush textureBrush;

        protected override void synchRootModel(Models.SceneModel model)
        {

            this.model.BrushesChanged += new SceneModel.ModelChangedHandler(model_BrushesChanged);
            this.controller.ActiveBrushChanged += new EditController.BrushHandler(controller_ActiveBrushChanged);
        }

        void controller_ActiveBrushChanged(SceneBrush brush)
        {
            this.Invalidate();
        }

        void model_BrushesChanged(SceneModel model)
        {
            this.Invalidate();
        }



        private void mapEditor_Paint(object sender, PaintEventArgs e)
        {

          /*  using (TextureBrush tb = new TextureBrush(this.testimg, new Rectangle(0, 0, 100, 100)))
            {
                e.Graphics.FillRectangle(tb, new Rectangle(160, 120, 100, 100));
            }*/
            this.drawBrushes(e.Graphics);
            this.drawCursor(e.Graphics);
          
        }

        Dictionary<string, Image> textureCache = new Dictionary<string, Image>();
        Dictionary<Image, TextureBrush> brushCache = new Dictionary<Image, TextureBrush>();

        List<string> loadedKeys = new List<string>();
        private Image getImage(string key)
        {
            if (textureCache.ContainsKey(key) == false)
            {
                this.loadedKeys.Add(key);
                textureCache[key] = Image.FromFile(key, true);
                return textureCache[key];
            }
            else
            {
                return textureCache[key];
            }
        }

        private TextureBrush getTextureBrush(Image key)
        {
            if (brushCache.ContainsKey(key) == false)
            {  
                
                    brushCache[key] = new TextureBrush(key);
                    return brushCache[key];

            }
            return brushCache[key];
        }

        private void drawBrushes(Graphics g)
        {

            this.zLabel.Text = this.controller.z_layer.ToString();

            List<SceneBrush> sortedBrushes;

            if (!this.paintersMode) { sortedBrushes = model.brushes.OrderBy(b => b.z).ToList(); }
            else
            {
                sortedBrushes = model.brushes.OrderBy(b => b.z).ToList();
            }


            

            foreach (SceneBrush brush in sortedBrushes)
            {
                int[] pos = transformToScreen(brush.x, brush.y, brush.z);
                int w = brush.w * unit_size;
                int h = brush.h * unit_size;
                Brush fillBrush;
                Brush transBrush;
                //Brush textureBrush;

                if (this.paintersMode == false)
                {
                    fillBrush = new SolidBrush(model.GetStyle(brush.styleName).UiColor);
                    transBrush = new SolidBrush(Color.FromArgb(64, model.GetStyle(brush.styleName).UiColor));
                }
                else
                {

                    fillBrush = this.getTextureBrush( this.getImage(this.model.GetStyle(brush.styleName).texture ));
                    transBrush = fillBrush;

                    TextureBrush tb = (TextureBrush)fillBrush;

                }

                int baseY = pos[1];
                bool is_current_layer = (brush.z == camera_z);

                if( is_current_layer) {
                        g.FillRectangle(fillBrush, pos[0], pos[1], w, h);

                        if (brush == this.controller.ActiveBrush)
                        {
                            g.DrawRectangle(Pens.GhostWhite, pos[0], pos[1], w, h);
                        }
                } else {

                     g.FillRectangle(transBrush, pos[0], pos[1], w, h);
                }

                
                {
                    Pen WallPen;

                    if (is_current_layer)
                    {
                        WallPen = new Pen(Brushes.Honeydew, 2.0f) { DashStyle = DashStyle.Dash };
                    } else {
                        WallPen = new Pen(Brushes.LightCyan, 1.0f ) { DashStyle = DashStyle.Dot };
                    }

                    {
                        if (brush.Walls[SceneBrush.NORTH_WALL])
                        {
                            g.DrawLine(WallPen, pos[0], pos[1], pos[0] + w, pos[1]);
                        }
                        if (brush.Walls[SceneBrush.EAST_WALL])
                        {
                            g.DrawLine(WallPen, pos[0] + w, pos[1], pos[0] + w, pos[1] + h);
                        }
                        if (brush.Walls[SceneBrush.SOUTH_WALL])
                        {
                            g.DrawLine(WallPen, pos[0], pos[1] + h, pos[0] + w, pos[1] + h);
                        }
                        if (brush.Walls[SceneBrush.WEST_WALL])
                        {
                            g.DrawLine(WallPen, pos[0], pos[1], pos[0], pos[1] + h);
                        }
                    }
                  
                }

                if (!(fillBrush is TextureBrush))
                {
                    fillBrush.Dispose();
                }

                if (!(transBrush is TextureBrush))
                {
                    transBrush.Dispose();
                }

            }

        }

        private int[] transformToScreen(int x, int y, int z, bool center = false)
        {

          

            int zDelta = z - this.camera_z;

            int scrX = (x - camera_x) * unit_size;
            int scrY = (y - camera_y - zDelta) * unit_size;

            if (center)
            {
                scrX += half_unit;
                scrY += half_unit;
            }


            int[] r = new int[2] { scrX, scrY };
            return r;
        }


        private void drawCursor(Graphics g)
        {
            int[] cScr = transformToScreen(cursor_x, cursor_y, this.camera_z, false);

            g.DrawLine(Pens.LightBlue, 0, cScr[1], this.Width, cScr[1]);
            g.DrawLine(Pens.LightBlue, cScr[0], 0, cScr[0], this.Height);

            Pen statePen = Pens.Blue;

            if (state == mapEditor.STATE_BRUSH_DEFINE_A ||
                state == mapEditor.STATE_BRUSH_DEFINE_B)
            {
                statePen = Pens.LimeGreen;

                if (state == mapEditor.STATE_BRUSH_DEFINE_B)
                {
                    this.drawTempBrush(g);
                }
            }

            g.DrawRectangle(statePen, cScr[0]-4, cScr[1]-4, 8, 8);
            
           
        }

        private void drawTempBrush(Graphics g)
        {
            int[] pos = transformToScreen(tmp_brush_x, tmp_brush_y, this.camera_z);
            int w = tmp_brush_w * unit_size;
            int h = tmp_brush_h * unit_size;

            g.DrawRectangle(Pens.White, pos[0], pos[1], w, h);
        }

        private void mapEditor_MouseMove(object sender, MouseEventArgs e)
        {

            this.Invalidate();
            cursor_x = (e.X / unit_size) + camera_x;
            cursor_y = (e.Y / unit_size) + camera_y;

            String curPosString = String.Format("Cusor : ({0},{1})", cursor_x, cursor_y);
            this.coordsLabel.Text = curPosString;

            if (state == mapEditor.STATE_BRUSH_DEFINE_B)
            {

                int w = cursor_x - def_origin_x;
                int h = cursor_y - def_origin_y;

                if (w < 0)
                {
                    tmp_brush_x = cursor_x;
                    tmp_brush_w = w * -1;
                }
                else
                {
                    tmp_brush_x = def_origin_x;
                    tmp_brush_w = w;
                }

                if (h < 0)
                {
                    tmp_brush_y = cursor_y;
                    tmp_brush_h = h * -1;
                }
                else
                {
                    tmp_brush_y = def_origin_y;
                    tmp_brush_h = h;
                }
              

            }
        }

        private void addBrush_Click(object sender, EventArgs e)
        {
            state = mapEditor.STATE_BRUSH_DEFINE_A;
        }

        int def_origin_x = 0;
        int def_origin_y = 0;
        int tmp_brush_x = 0;
        int tmp_brush_y = 0;
        int tmp_brush_w = 0;
        int tmp_brush_h = 0;

        int nextId = 0;

        private void commitBrush()
        {
            SceneBrush newBrush = new SceneBrush() { x = tmp_brush_x, y = tmp_brush_y, w = tmp_brush_w, h = tmp_brush_h, orientation = 0, z = camera_z, styleName = this.controller.ActiveStyleKey };


            if (this.controller.DrawMode == EditController.DRAWMODE_FLOOR)
            {
                newBrush.walls = new bool[] { false, false, false, false };
                newBrush.name = "floor " + this.nextId.ToString();
            }
            else
            {
                newBrush.name = "wall" + this.nextId.ToString();
            }

            this.nextId += 1;

           
            this.model.AddBrush(newBrush);
            this.controller.ActiveBrush = newBrush;

            this.brushLabel.Text = String.Format("Number of Brushes: {0}", this.model.brushes.Count);
        }

        private void statusStrip1_Click(object sender, EventArgs e)
        {

       

        }

        private void mapEditor_Click(object sender, EventArgs e)
        {
            
            if (state == mapEditor.STATE_BRUSH_DEFINE_A)
            {

                def_origin_x = cursor_x;
                def_origin_y = cursor_y;
                tmp_brush_x = cursor_x;
                tmp_brush_y = cursor_y;
                tmp_brush_w = 1;
                tmp_brush_h = 1;
                state = mapEditor.STATE_BRUSH_DEFINE_B;
                return;
            }

            if (state == mapEditor.STATE_BRUSH_DEFINE_B)
            {
                this.commitBrush();
                state = 0;
                return;
            }
        }

        private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void upLayer_Click(object sender, EventArgs e)
        {
            this.camera_z += 1;
            this.controller.z_layer += 1;
            this.Invalidate();
        }

        private void downLayer_Click(object sender, EventArgs e)
        {
            this.camera_z -= 1;
            this.controller.z_layer -= 1;
            this.Invalidate();
        }

        private void drawModeSelector_TextChanged(object sender, EventArgs e)
        {
            if (drawModeSelector.Text.Equals("floors"))
                this.controller.DrawMode = EditController.DRAWMODE_FLOOR;
            if (drawModeSelector.Text.Equals("walls"))
                this.controller.DrawMode = EditController.DRAWMODE_WALL;
        }

        bool paintersMode = false;
        private void previewModeSelector_TextChanged(object sender, EventArgs e)
        {
            if (previewModeSelector.Text.Equals("onion") ) {
                this.paintersMode = false;
            } 

            if( previewModeSelector.Text.Equals("texture") ) {

                this.paintersMode = true;
            }

            this.Invalidate();

        }

        private void mapEditor_Load(object sender, EventArgs e)
        {

        }

        private void mapEditor_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                int center_x = (this.Width / unit_size) / 2;
                int center_y = (this.Height / unit_size) / 2;

           //     int delta_x = cursor_x - (camera_x+center_x;
             //   int delta_y = cursor_y - (camera_y+center_y);
                camera_x  = cursor_x - center_x;
                camera_y  = cursor_y - center_y;

                cursor_x = (e.X / unit_size) + camera_x;
                cursor_y = (e.Y / unit_size) + camera_y;
            }
        }
    }
}
