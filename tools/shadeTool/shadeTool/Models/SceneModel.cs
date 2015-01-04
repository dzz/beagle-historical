using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace shadeTool.Models
{
    public class SceneBrush
    {
        string name = "Brush";
        public int x, y, z1, z2, w, h, orientation;


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

        public List<SceneBrush> brushes { get { return this._brushes; } }
    }
}
