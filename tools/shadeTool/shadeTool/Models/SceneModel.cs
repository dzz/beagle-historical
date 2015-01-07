using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace shadeTool.Models
{
    public class BrushStyle
    {
        public const int GEOM_CONVEX = 0;
        public const int GEOM_CONCAVE = 1;

        public Color UiColor = Color.Purple;
        public int Geometry = 1;
    }

    public class SceneBrush
    {
        public string name = "default_brush";
        public int x, y, z1, z2, w, h, orientation;

        public const int NORTH_WALL = 0;
        public const int EAST_WALL = 1;
        public const int SOUTH_WALL = 2;
        public const int WEST_WALL = 3;
        public const int FLOOR = 4;

        public bool[] walls = new bool[5] { true, true, true, true, true };

        public bool[] Walls { get { return walls; } set { walls = value; } }
        public String styleName = "brush_default_style";

  
        public override string ToString()
        {
            return name;
        }

        public int[] getCenter()
        {
            return new int[2] { x+(w/2), y+(h/2) };
        }
    }

    public class SceneModel
    {
        private List<SceneBrush> _brushes = new List<SceneBrush>();
        private Dictionary<String, BrushStyle> _styles = new Dictionary<string, BrushStyle>();
        
        public List<SceneBrush> brushes { get { return this._brushes; } }
        public Dictionary<String, BrushStyle> styles { get { return this._styles; } }

        public delegate void ModelChangedHandler(SceneModel model);
        public event ModelChangedHandler StylesChanged;
        public event ModelChangedHandler BrushesChanged;

        public void AddBrush(SceneBrush brush)
        {
      
            this.brushes.Add(brush);

            if (BrushesChanged != null)
            {
                BrushesChanged(this);
            }
            
        }

        public void DelBrush(SceneBrush brush)
        {
            this.brushes.Remove(brush);

            if (BrushesChanged != null)
            {
                BrushesChanged(this);
            }

        }

        public void SetStyle(String key, BrushStyle style)
        {
            this.styles[key] = style;

            if (StylesChanged != null)
            {
                StylesChanged(this);
            }
        }

        public BrushStyle GetStyle(String key)
        {
            try
            {
                return this.styles[key];
            }
            catch
            {
                return new BrushStyle();
            }
        }

        public SceneModel()
        {
            styles.Add("brush_default_style", new BrushStyle());
        }

    }
}
