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
using System.IO;

namespace shadeTool.Views
{
    public partial class mapEditor : ModelControllerView
    {
        

        int camera_x    = 0;
        int camera_y    = 0;
        int camera_z    = 0;
        int cursor_x    = 0;
        int cursor_y    = 0;

        int state = 0;

        const int STATE_NONE = 0;
        const int STATE_BRUSH_DEFINE_A = 1;
        const int STATE_BRUSH_DEFINE_B = 2;

        Timer TextureGarbageCollectTimeout = new Timer();

        public mapEditor()
        {
            InitializeComponent();
            this.DoubleBuffered = true;

            TextureGarbageCollectTimeout.Interval = 1000 * 60 * 15;
            TextureGarbageCollectTimeout.Tick += new EventHandler(GarbageCollectTimeout_Tick);
            TextureGarbageCollectTimeout.Enabled = true;

      
        }

        void GarbageCollectTimeout_Tick(object sender, EventArgs e)
        {
            this.textureCache.Clear();
            this.brushCache.Clear();
        }

        protected override void synchRootModel(Models.SceneModel model)
        {

            this.model.BrushesChanged += new SceneModel.ModelChangedHandler(model_BrushesChanged);
            this.controller.ActiveBrushChanged += new EditController.BrushHandler(controller_ActiveBrushChanged);
            this.model.SettingsChanged += new SceneModel.ModelChangedHandler(model_SettingsChanged);
            this.controller.ActiveBrushModified += new EditController.ActiveBrushModifiedHandler(controller_ActiveBrushModified);
            this.dirty = true;
        }

        void controller_ActiveBrushModified(SceneBrush brush)
        {
            this.dirty = true;
            this.Invalidate();
        }

        void model_SettingsChanged(SceneModel model)
        {
            this.dirty = true;
            this.Invalidate();
        }

        void controller_ActiveBrushChanged(SceneBrush brush)
        {
            this.dirty = true;
            this.Invalidate();
        }

        void model_BrushesChanged(SceneModel model)
        {
            this.dirty = true;
            this.Invalidate();
        }


        Bitmap bg = new Bitmap(1920, 1080);
        Bitmap minimap = new Bitmap(150, 150);

        private void mapEditor_Paint(object sender, PaintEventArgs e)
        {
            //this.BackgroundImage = bg;

            this.refreshBackground();
            e.Graphics.DrawImage(bg,0,0);

            
            //this.drawBrushes(e.Graphics);
            
            this.drawCursor(e.Graphics);
            e.Graphics.DrawImage(minimap, this.Width-minimap.Width, 0);
          
        }

        private void refreshBackground()
        {
            if (dirty)
            {
                Graphics g = Graphics.FromImage(bg);
                g.Clear(Color.Transparent);
                this.drawBrushes(g);
                g.Dispose();

                /** minimap hack **/
                int oldsize = this.model.world_unit_size;

                this.model.world_unit_size = 2;

                bool oldPainters = this.paintersMode;
                bool oldPaintcurrent = this.paintCurrentOnly;

                int center = 16;

                this.camera_x -= center;
                this.camera_y -= center;

                this.paintersMode = false;
                this.paintCurrentOnly = false;
                Graphics g2 = Graphics.FromImage(minimap);
                g2.Clear(Color.Black);
                this.drawBrushes(g2);
                g2.Dispose();

                this.model.world_unit_size = oldsize;

                this.paintersMode = oldPainters;
                this.paintCurrentOnly = oldPaintcurrent;

                this.camera_x += center;
                this.camera_y += center;

            }
            dirty = false;
        }

        Dictionary<string, Image> textureCache = new Dictionary<string, Image>();
        Dictionary<Image, TextureBrush> brushCache = new Dictionary<Image, TextureBrush>();

        private Image getImage(string key)
        {

           
            if (key == null)
                key = "";

            if (textureCache.ContainsKey(key) == false)
            {
                try
                {

                        textureCache[key] = Image.FromFile(this.model.getTexturePath()+key, true);

                }
                catch
                {
                    Bitmap bmp = new Bitmap(64, 64);
                    Graphics g = Graphics.FromImage(bmp);
                    g.Clear(Color.Transparent);
                    g.DrawString("[notex]", this.Font, new SolidBrush(Color.Red), new Point(0, 0));
                    textureCache[key] = bmp;
                }
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

            for (int i = 0; i < model.brushes.Count; ++i)
                model.brushes[i].sortOrder = i;

            if (!this.paintersMode) { sortedBrushes = model.brushes.OrderBy(b => b.z).ToList(); }
            else
            {
                sortedBrushes = model.brushes.OrderBy(b => b.z).ThenBy( b=>b.billboard ? (b.y+b.h) : (b.y) ).ThenBy( b=>b.type).ToList();
            }

            foreach (SceneBrush brush in sortedBrushes)
            {
                int[] pos = transformToScreen(brush.x, brush.y, brush.z);
               
         
                int w = brush.w * this.model.world_unit_size;
                int w2 = w / 2;
                int h = brush.h * this.model.world_unit_size;
                int h2 = h/2;
                Brush fillBrush;
                Brush transBrush;
                //Brush textureBrush;

                BrushStyle style = model.GetStyle( brush.styleName );


                int cx = this.Width / 2;
                int cy = this.Height / 2;

                int[] para_pos = new int[2] { (int)((double)(pos[0]-cx+w2)*style.parallax)+cx-w2,(int)((double)(pos[1]-cy+h2)*style.parallax)+cy-h2 };

                    if (this.paintersMode == false)
                    {
                        fillBrush = new SolidBrush(style.UiColor);
                        transBrush = new SolidBrush(Color.FromArgb(64, style.UiColor));
                    }
                    else
                    {

                        fillBrush = this.getTextureBrush(this.getImage(style.texture));

                        if (this.paintCurrentOnly == false)
                            transBrush = fillBrush;
                        else
                        {
                            transBrush = new SolidBrush(Color.FromArgb(64, style.UiColor));
                        }

                        TextureBrush tb = (TextureBrush)fillBrush;

                        tb.ResetTransform();

               

                        if (style.origin_mode == BrushStyle.origin_mode_local)
                        {
                            if (style.parallax_object == false)
                            {
                                tb.TranslateTransform((float)pos[0] * (float)style.parallax, (float)pos[1] * (float)style.parallax);
                            }
                            else
                            {
                                tb.TranslateTransform((float)para_pos[0], (float)para_pos[1]);
                            }
                        }

                        if (style.uv_mode == BrushStyle.uv_mode_scale)
                        {
                            double sx = (double)w / (double)tb.Image.Width;
                            double sy = (double)h / (double)tb.Image.Height;

                            tb.ScaleTransform((float)sx, (float)sy);
                        }

                    }

                int baseY = pos[1];
                bool is_current_layer = (brush.z == camera_z);

                if (brush.type == SceneBrush.WALL_BRUSH)
                {
                    Pen wallPen = null;
                    if (is_current_layer)
                        if (this.controller.ActiveBrush != brush)
                            wallPen = Pens.White;
                        else
                            wallPen = Pens.Red;
                    else
                        wallPen = Pens.Gray;


                    g.DrawLine(wallPen, pos[0], pos[1], pos[0] + w, pos[1] + h);

                    double nx = -h;
                    double ny = w;

                    double len = Math.Sqrt(nx * nx + ny * ny);

                    if (len > double.Epsilon)
                    {
                        nx /= len;
                        ny /= len;

                        nx *= 32;
                        ny *= 32;

                        using (Pen normalPen = new Pen(Color.BlueViolet, 12))
                        {
                            if (is_current_layer)
                            {

                                normalPen.EndCap = LineCap.ArrowAnchor;


                                int midX = (pos[0] + (w / 2));
                                int midY = (pos[1] + (h / 2));

                                g.DrawLine(normalPen, midX, midY, midX + (int)nx, midY + (int)ny);
                            }
                        }
                    }
                }

 
                if( is_current_layer) {

                    if (brush.type == SceneBrush.FLOOR_BRUSH)
                    {
                        if (style.parallax_object)
                        {
                            g.FillRectangle(fillBrush, para_pos[0], para_pos[1], w, h);
                            Pen p = new Pen(Brushes.Azure, 2);
                            g.DrawRectangle(p, pos[0], pos[1],w,h);
                            p.Dispose();
                        }
                        else
                        {
                            g.FillRectangle(fillBrush, pos[0], pos[1], w, h);
                        }
                    }

                        if (brush == this.controller.ActiveBrush)
                        {
                            g.DrawRectangle(Pens.GhostWhite, pos[0], pos[1], w, h);
                        }

                        


                } else {
                    if (brush.type == SceneBrush.FLOOR_BRUSH)
                    {
                        if (style.parallax_object)
                        {
                            g.FillRectangle(transBrush, para_pos[0], para_pos[1], w, h);
                           if(is_current_layer)
                               g.DrawRectangle(Pens.LemonChiffon, pos[0], pos[1], w, h);
                        }
                        else
                        {
                            g.FillRectangle(transBrush, pos[0], pos[1], w, h);
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

                if (brush.orientation != SceneBrush.FLOOR && brush.type==SceneBrush.WALL_BRUSH)
                {
                    using( Pen rampPen = new Pen( Color.FromArgb(128, Color.Red) , 16) ) {
                        rampPen.EndCap = LineCap.ArrowAnchor;


                        int nx = -brush.h;
                        int ny = brush.w;
                        int len = 64;
                        switch (brush.orientation)
                        {
                            case SceneBrush.NORTH_WALL:
                                g.DrawLine(rampPen, pos[0], pos[1], pos[0], pos[1] - len);
                                break;
                            case SceneBrush.SOUTH_WALL:
                                g.DrawLine(rampPen, pos[0], pos[1], pos[0], pos[1] + len);
                                break;
                        }
                    }
                }
             
            }
           
        }

        private int[] transformToScreen(int x, int y, int z)
        {
            int zDelta = z - this.camera_z;

            int scrX = (x - camera_x) * this.model.world_unit_size;
            int scrY = (y - camera_y - zDelta) * this.model.world_unit_size;


            int[] r = new int[2] { scrX, scrY };
            return r;
        }


        private void drawCursor(Graphics g)
        {
            int[] cScr = transformToScreen(cursor_x, cursor_y, this.camera_z);

            g.DrawLine(Pens.LightBlue, 0, cScr[1], this.Width, cScr[1]);
            g.DrawLine(Pens.LightBlue, cScr[0], 0, cScr[0], this.Height);

            Pen statePen = Pens.Blue;

            if (state == mapEditor.STATE_BRUSH_DEFINE_A ||
                state == mapEditor.STATE_BRUSH_DEFINE_B)
            {
                if (state == mapEditor.STATE_BRUSH_DEFINE_B)
                {
                    this.drawTempBrush(g);
                }
            }

            g.DrawRectangle(statePen, cScr[0]-4, cScr[1]-4, 8, 8);


            List<SceneEntity> ses = this.model.entities.FindAll(o => o.z == camera_z);

            foreach (SceneEntity entity in ses)
            {
                int[] pos = this.transformToScreen(entity.x, entity.y, camera_z);

                Image sprite = this.getEntityImage(entity);

                g.DrawImage(this.getEntityImage(entity), pos[0] - (this.model.world_unit_size/2), pos[1]-(this.model.world_unit_size/2), sprite.Width, sprite.Height);
            }

            g.DrawRectangle(Pens.Orange, ( (EntityCursorX-camera_x) * this.model.world_unit_size) - this.model.world_unit_size/2 , ( (EntityCursorY-camera_y) * this.model.world_unit_size) - this.model.world_unit_size/2, this.model.world_unit_size, this.model.world_unit_size);
        }

        Dictionary<string, Image> imgCache = new Dictionary<string, Image>();

        private Image getEntityImage(SceneEntity e)
        {

   
                string lookup = this.model.project_root + "script\\preview\\" + e.script + ".png";

                if(imgCache.ContainsKey(lookup))
                    return imgCache[lookup];

                if (File.Exists(lookup))
                {
                    try
                    {
                        imgCache[lookup] = Image.FromFile(lookup);
                    }
                    catch
                    {
                        imgCache[lookup] = null;
                    }
                }

                if ( imgCache.ContainsKey(lookup) == false )
                {
                    string def_lookup = this.model.project_root + "script\\preview\\default.png";

                    if(imgCache.ContainsKey(def_lookup) == false)
                        imgCache[def_lookup] = Image.FromFile(def_lookup);

                    imgCache[lookup] = imgCache[def_lookup];
                }


            return imgCache[lookup];


        }

        private void drawTempBrush(Graphics g)
        {
            int[] pos = transformToScreen(tmp_brush_x, tmp_brush_y, this.camera_z);
            int w = tmp_brush_w * this.model.world_unit_size;
            int h = tmp_brush_h * this.model.world_unit_size;

            if (this.controller.DrawMode == EditController.DRAWMODE_FLOOR)
            {
                BrushStyle style=this.model.styles[this.controller.ActiveStyleKey];
                if (style != null)
                {
                    TextureBrush tb = this.getTextureBrush(this.getImage(style.texture));

                    tb.ResetTransform();
                    g.SmoothingMode = SmoothingMode.None;

                    if (style.origin_mode == BrushStyle.origin_mode_local)
                    {
                        tb.TranslateTransform(pos[0], pos[1]);
                    }

                    if (style.uv_mode == BrushStyle.uv_mode_scale)
                    {
                        double sx = (double)w / (double)tb.Image.Width;
                        double sy = (double)h / (double)tb.Image.Height;

                        tb.ScaleTransform((float)sx, (float)sy);
                    }

                    g.FillRectangle(tb, pos[0], pos[1], w, h);
                }

                g.DrawRectangle(Pens.White, pos[0], pos[1], w, h);
                
            }
            else
            {
                g.DrawLine(Pens.White, pos[0], pos[1], pos[0] + w, pos[1] + h);

                double nx = -h;
                double ny = w;

                double len = Math.Sqrt(nx * nx + ny * ny);

                if ( len > double.Epsilon )
                {
                    nx /= len;
                    ny /= len;

                    nx *= 32;
                    ny *= 32;

                    using (Pen normalPen = new Pen(Color.AliceBlue, 12))
                    {
                        normalPen.EndCap = LineCap.ArrowAnchor;

                        int midX = (pos[0] + (w / 2));
                        int midY = (pos[1] + (h / 2));

                        g.DrawLine(normalPen, midX, midY, midX + (int)nx, midY + (int)ny);
                    }
                }
            }
        }

        private void mapEditor_MouseMove(object sender, MouseEventArgs e)
        {

            this.Invalidate();
            cursor_x = (e.X / this.model.world_unit_size) + camera_x;
            cursor_y = (e.Y / this.model.world_unit_size) + camera_y;

            String curPosString = String.Format("Cusor : ({0},{1})", cursor_x, cursor_y);
            this.coordsLabel.Text = curPosString;

            if (state == mapEditor.STATE_BRUSH_DEFINE_B)
            {

                bool walls = (this.controller.DrawMode == EditController.DRAWMODE_WALL);

                if (walls)
                {
                    tmp_brush_x = def_origin_x;
                    tmp_brush_y = def_origin_y;
                    tmp_brush_w = cursor_x - def_origin_x;
                    tmp_brush_h = cursor_y - def_origin_y;
                }

                else
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

                    if (h < 0 & !walls)
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

        }

        private void addBrush_Click(object sender, EventArgs e)
        {
            if (addBrushButton.Checked)
            {
                anchorLabel.Visible = true;
                state = mapEditor.STATE_BRUSH_DEFINE_A;
                this.lockControlsForState();
            }
            else
            {
                anchorLabel.Visible = false;
                state = mapEditor.STATE_NONE;
                this.unlockControlsForState();
            }
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
            dirty = true;
            SceneBrush newBrush = new SceneBrush() { x = tmp_brush_x, y = tmp_brush_y, w = tmp_brush_w, h = tmp_brush_h, orientation = SceneBrush.FLOOR, z = camera_z, styleName = this.controller.ActiveStyleKey };


            if (this.controller.DrawMode == EditController.DRAWMODE_FLOOR)
            {
                //newBrush.walls = new bool[] { false, false, false, false };
                newBrush.name = "floor " + this.nextId.ToString();
                newBrush.type = SceneBrush.FLOOR_BRUSH;
            }
            else
            {
                newBrush.name = "wall" + this.nextId.ToString();
                newBrush.type = SceneBrush.WALL_BRUSH;
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
            

        }

        private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void upLayer_Click(object sender, EventArgs e)
        {
            this.dirty = true;
            this.state = STATE_NONE;
            this.addBrushButton.Checked = false;

            this.camera_z += 1;
            this.controller.z_layer += 1;
            this.Invalidate();
        }

        private void downLayer_Click(object sender, EventArgs e)
        {
            this.dirty = true;
            this.state = STATE_NONE;
            this.addBrushButton.Checked = false;

            this.camera_z -= 1;
            this.controller.z_layer -= 1;
            this.Invalidate();
        }

        //private void drawModeSelector_TextChanged(object sender, EventArgs e)
        //{
        //    if (drawModeSelector.Text.Equals("floors"))
        //        this.controller.DrawMode = EditController.DRAWMODE_FLOOR;
        //    if (drawModeSelector.Text.Equals("walls"))
        //        this.controller.DrawMode = EditController.DRAWMODE_WALL;
        //    SendKeys.Send("{ESC}");

        //    this.Focus();
        //}

        bool paintersMode = true;
        bool paintCurrentOnly = true;

       /* private void previewModeSelector_TextChanged(object sender, EventArgs e)
        {
            this.paintCurrentOnly = false;
            if (previewModeSelector.Text.Equals("onion") ) {
                this.paintersMode = false;
            } 

            if( previewModeSelector.Text.Equals("texture all") ) {

                this.paintersMode = true;
            }

            if (previewModeSelector.Text.Equals("texture this layer"))
            {

                this.paintersMode = true;
                this.paintCurrentOnly = true;
            }

            this.Invalidate();
            SendKeys.Send("{ESC}");

            this.Focus();
        }
        */

        private void mapEditor_Load(object sender, EventArgs e)
        {

        }

        public int EntityCursorX = 0;
        public int EntityCursorY = 0;

        public Boolean dirty = false;

        private void mapEditor_MouseDown(object sender, MouseEventArgs e)
        {
            dirty = true;
            this.Focus();
            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
      

                this.addBrushButton.Checked = false;
                this.unlockControlsForState();


                int center_x = (this.Width / this.model.world_unit_size) / 2;
                int center_y = (this.Height / this.model.world_unit_size) / 2;

           //     int delta_x = cursor_x - (camera_x+center_x;
             //   int delta_y = cursor_y - (camera_y+center_y);
                camera_x  = cursor_x - center_x;
                camera_y  = cursor_y - center_y;

                cursor_x = (e.X / this.model.world_unit_size) + camera_x;
                cursor_y = (e.Y / this.model.world_unit_size) + camera_y;
            }

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                if (state == mapEditor.STATE_NONE)
                {

                    EntityCursorX = cursor_x;
                    EntityCursorY = cursor_y;

                    this.loadTileEntityList();

                    List<SceneBrush> thisLayerBrushes = this.model.brushes.FindAll(x => x.z == camera_z);


                    foreach (var b in thisLayerBrushes)
                    {
                        if (cursor_x >= b.x &&
                            cursor_x < (b.x + b.w) &&
                            cursor_y > b.y &&
                            cursor_y < (b.y + b.h))
                        {
                            this.controller.ActiveBrush = b;
                            return;
                        }
                    }
                }

                if (state == mapEditor.STATE_BRUSH_DEFINE_A)
                {
                    this.lockControlsForState();
                    anchorLabel.Visible = false;
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
                    if (addBrushButton.Checked)
                    {
                        state = STATE_BRUSH_DEFINE_A;
                    }
                    else
                    {
                        state = STATE_NONE;
                        this.unlockControlsForState();
                    }
                    return;
                }
            }
        }

        private void previewModeSelector_Click(object sender, EventArgs e)
        {

        }

        private ToolStripItem[] getStateLockableControls()
        {
            return new ToolStripItem[3]{
            
                 (ToolStripItem)this.upLayerButton,
                 (ToolStripItem)this.downLayerButton,
                 (ToolStripItem)this.selectBrushButton
        };
        }
        private void lockControlsForState()
        {
            foreach (ToolStripItem comp in this.getStateLockableControls())
            {
                comp.Enabled = false;
            }
   
        }

        private void unlockControlsForState()
        {
            foreach (ToolStripItem comp in this.getStateLockableControls())
            {
                comp.Enabled = true;
            }
        }

        private void mapEditor_KeyDown(object sender, KeyEventArgs e)
        {
            dirty = true;
          //  MessageBox.Show("kley");

         //   MessageBox.Show(drawModeSelector.SelectedIndex.ToString());
            if (e.KeyCode == Keys.Escape)
            {
                if (this.state == STATE_BRUSH_DEFINE_A)
                {
                    this.state = STATE_NONE;
                    this.addBrushButton.Checked = false;
                    this.unlockControlsForState();
                }
                if (this.state == STATE_BRUSH_DEFINE_B)
                    this.state = STATE_BRUSH_DEFINE_A;
            }

            if (e.KeyCode == Keys.A)
            {
                this.state = STATE_BRUSH_DEFINE_A;
                this.addBrushButton.Checked = true;
                this.anchorLabel.Visible = true;
            }

            if (e.KeyCode == Keys.W)
            {
                this.upLayer_Click(null, null);
            }

            if (e.KeyCode == Keys.S)
            {
                this.downLayer_Click(null, null);
            }
        }

        private void mapEditor_KeyPress(object sender, KeyPressEventArgs e)
        {
          
        }

        private void toolStrip1_KeyDown(object sender, KeyEventArgs e)
        {
           //this.mapEditor_KeyDown(sender, e);
        }

        private void previewModeSelector_KeyDown(object sender, KeyEventArgs e)
        {
          //  this.mapEditor_KeyDown(sender, e);
            e.Handled = true;
        }

        private void drawModeFloorsButton_CheckedChanged(object sender, EventArgs e)
        {
            dirty = true;
            if (drawModeFloorsButton.Checked == true)
            {
                drawModeWallsButton.Checked = false;

                this.controller.DrawMode = EditController.DRAWMODE_FLOOR;
            }

        }

        private void drawModeWallsButton_CheckedChanged(object sender, EventArgs e)
        {
            dirty = true;
            if (drawModeWallsButton.Checked == true)
            {
                drawModeFloorsButton.Checked = false;
                this.controller.DrawMode = EditController.DRAWMODE_WALL;
            }
        }

        private void renderModeOnion_CheckedChanged(object sender, EventArgs e)
        {
            dirty = true;
            if (renderModeOnion.Checked == true)
            {
                renderModeTextureAll.Checked = false;
                renderModeTextureCurrent.Checked = false;

                this.paintersMode = false;
                this.paintCurrentOnly = false;
          
            }

        }

        private void renderModeTextureAll_CheckedChanged(object sender, EventArgs e)
        {
            dirty = true;
            if (renderModeTextureAll.Checked == true)
            {
                renderModeOnion.Checked = false;
                renderModeTextureCurrent.Checked = false;

                this.paintersMode = true;
                this.paintCurrentOnly = false;

            }
        }

        private void renderModeTextureCurrent_CheckedChanged(object sender, EventArgs e)
        {
            dirty = true;
            if (renderModeTextureCurrent.Checked == true)
            {
                renderModeTextureAll.Checked = false;
                renderModeOnion.Checked = false;

                this.paintersMode = true;
                this.paintCurrentOnly = true;

            }
        }

        private void toolStripButton3_CheckedChanged(object sender, EventArgs e)
        {
            this.panel1.Visible = toolStripButton3.Checked;
        }

        public void synchTileEntity(SceneEntity ent)
        {
           
            this.loadTileEntityList();
            if (ent != null)
            {
                this.entityDefBox.Text = ent.streamInitCode.Replace("\n", "\r\n");
                this.entityName.Text = ent.name;
            }
        }

        public void loadTileEntityList()
        {
            List<SceneEntity> tel = this.model.entities.FindAll(o => o.x == EntityCursorX && o.y == EntityCursorY && o.z == camera_z);

            this.entityListing.Items.Clear();
            this.entityListing.Items.AddRange(tel.ToArray());

            if (tel.Contains(this.activeEntity) == false)
            {
                this.activeEntity = null;
                this.entityDefBox.Text = "";
                this.entityName.Text = "";

                if (tel.Count > 0)
                    this.entityListing.SelectedIndex = 0;

                this.ActiveControl = null;
            }
            else
            {

                this.entityListing.SelectedItem = activeEntity;
            }

        }

        private void addEntity_Click(object sender, EventArgs e)
        {
            SceneEntity ent = new SceneEntity();

            ent.x = EntityCursorX;
            ent.y = EntityCursorY;
            ent.z = camera_z;

            EntityTypeSelect ets = new EntityTypeSelect();

            ets.setModel(this.model);
            ets.ShowDialog();

            ent.script = ets.value;

            if (ent.script != "")
            {
                StreamReader sr = File.OpenText(this.model.project_root + "script/" + ent.script + ".default");

                string s = sr.ReadToEnd();

                sr.Close();

                ent.streamInitCode = s;

            }

            ent.name = ent.script + " instance";

            this.model.entities.Add(ent);
            this.Invalidate();

            this.synchTileEntity(ent);
        }

        private SceneEntity activeEntity = null;
        private void entityListing_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (entityListing.SelectedIndex != -1 && entityListing.SelectedItem!=activeEntity )
            {
                activeEntity = (SceneEntity)this.entityListing.SelectedItem;
                this.synchTileEntity(activeEntity);
            }
        }

        private void entityName_TextChanged(object sender, EventArgs e)
        {
            if (this.activeEntity != null)
            {
                this.activeEntity.name = entityName.Text;
                this.synchTileEntity(this.activeEntity);
            }
        }

        private void entityDefBox_TextChanged(object sender, EventArgs e)
        {
            if (this.activeEntity != null)
            {
                this.activeEntity.streamInitCode = entityDefBox.Text.Replace("\r\n", "\n");
            }
        }

        private void delEntity_Click(object sender, EventArgs e)
        {
            if (this.activeEntity != null)
            {
                this.model.entities.Remove(this.activeEntity);
                this.activeEntity = null;

            }

            
            this.synchTileEntity(null);
        }

        private void entityDefBox_MouseLeave(object sender, EventArgs e)
        {
            this.ActiveControl = null;
        }

        private void entityName_KeyDown(object sender, KeyEventArgs e)
        {
            
            if (e.KeyCode == Keys.Enter)
                this.ActiveControl = null;
        }

    }
}
