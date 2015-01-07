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
  
        public mapEditor()
        {
            InitializeComponent();

          //  this.WindowState = FormWindowState.Maximized;
            this.DoubleBuffered = true;

        }


        private void mapEditor_Paint(object sender, PaintEventArgs e)
        {
            this.drawBrushes(e.Graphics);
            this.drawCursor(e.Graphics);
          
        }

        private void drawBrushes(Graphics g)
        {
            foreach (SceneBrush brush in model.brushes)
            {
                int[] pos = transformToScreen(brush.x, brush.y);
                int w = brush.w * unit_size;
                int h = brush.h * unit_size;
                SolidBrush fillBrush = new SolidBrush(model.GetStyle(brush.styleName).UiColor);
                g.FillRectangle(fillBrush, pos[0], pos[1], w, h);

                g.DrawRectangle(Pens.ForestGreen, pos[0], pos[1], w, h);
                int baseY = pos[1];

                for (int i = 0; i < 2; ++i )
                {
                    pos[1] = baseY - i*unit_size;
                    if (brush.z1 == camera_z)
                    {












                        Pen WallPen = new Pen(Brushes.Honeydew, 2.2f) { DashStyle = DashStyle.DashDotDot };
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

                g.DrawString(brush.name, this.Font, Brushes.Black, pos[0], pos[1]);
            }
        }

        private int[] transformToScreen(int x, int y, bool center = false)
        {

            int scrX = (x - camera_x) * unit_size;
            int scrY = (y - camera_y) * unit_size;

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
            int[] cScr = transformToScreen(cursor_x, cursor_y, false);

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
            int[] pos = transformToScreen(tmp_brush_x, tmp_brush_y);
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

        private void commitBrush()
        {
            SceneBrush newBrush = new SceneBrush() { x = tmp_brush_x, y = tmp_brush_y, w = tmp_brush_w, h = tmp_brush_h, orientation = 0, z1 = camera_z, z2 = camera_z, styleName = this.controller.ActiveStyleKey };

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
        }

        private void downLayer_Click(object sender, EventArgs e)
        {
            this.camera_z -= 1;
            this.controller.z_layer -= 1;
        }
    }
}
