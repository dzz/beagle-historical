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
        string name = "Brush";
        public int x, y, z1, z2, w, h, orientation;

        bool isStaircase = false;
        String styleName = "";

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

        public void SetStyle(String key, BrushStyle style)
        {
            this.styles[key] = style;

            if (StylesChanged != null)
            {
                StylesChanged(this);
            }
        }

        public SceneModel()
        {
            styles.Add("brush_default_style", new BrushStyle());
        }
    }
}
