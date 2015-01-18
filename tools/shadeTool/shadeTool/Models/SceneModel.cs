﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;
using System.Xml.Serialization;

namespace shadeTool.Models
{
    public class BrushStyle 
    {
        public string texture { get; set; }
        public string shader { get; set; } 
        public const int GEOM_CONVEX = 0;
        public const int GEOM_CONCAVE = 1;

        [XmlIgnore]
        public Color UiColor { get; set; }

        public int UIC_R{   get{ return UiColor.R; }
                            set{ UiColor = Color.FromArgb( value, UiColor.G, UiColor.B ); }
        }

        public int UIC_G{   get{ return UiColor.G; }
                            set{ UiColor = Color.FromArgb( UiColor.R, value, UiColor.B ); }
        }

        public int UIC_B{   get{ return UiColor.B; }
                            set{ UiColor = Color.FromArgb( UiColor.R, UiColor.G, value ); }
        }

        public int uv_mode { get; set; }
        public int origin_mode { get; set; }

        public const int uv_mode_repeat = 0;
        public const int uv_mode_scale = 1;

        public const int origin_mode_local = 0;
        public const int origin_mode_global = 1;

        public int origin_x { get; set; }
        public int origin_y { get; set; }


        public BrushStyle()
        {
            UiColor = Color.Purple;
        }
    }

    public class SceneBrush
    {

        public bool billboard { get; set; }

        public const int NORTH_WALL = 0;
        public const int EAST_WALL = 1;
        public const int SOUTH_WALL = 2;
        public const int WEST_WALL = 3;
        public const int FLOOR = 4;

        public string name { get; set; }
        public string styleName { get; set; }

        public const int FLOOR_BRUSH = 0;
        public const int WALL_BRUSH = 1;

        public int type { get; set; }

        public int x { get;set;  }
        public int y { get; set; }
        public int z { get; set; }
        public int w { get; set; }
        public int h { get; set; }

        public int orientation { get; set; }

        public bool[] walls = new bool[5] { true, true, true, true, true };
        public bool[] Walls { get { return walls; } set { walls = value; } }


        public int sortOrder { get; set; }

        public SceneBrush()
        {
            styleName       = "default_brush_style";
            name            = "brush";
            orientation = FLOOR;
        }

        public override string ToString()
        {
            if (this.type == SceneBrush.FLOOR_BRUSH)
            {
                String billboardString = this.billboard ? "(billboard)" : "(floor)";

                return String.Format("{1}{0}:{2}", billboardString, name, styleName);
            }
            else
            {
                return String.Format("(wall): {0}", this.name);
            }
        }

        public int[] getCenter()
        {
            return new int[2] { x+(w/2), y+(h/2) };
        }
    }

    public class XmlDictionary<T, V> : Dictionary<T, V>, IXmlSerializable
    {
        [XmlType("Entry")]
        public struct Entry
        {
            public Entry(T key, V value) : this() { Key = key; Value = value; }
            [XmlElement("Key")]
            public T Key { get; set; }
            [XmlElement("Value")]
            public V Value { get; set; }
        }
        System.Xml.Schema.XmlSchema IXmlSerializable.GetSchema()
        {
            return null;
        }
        void IXmlSerializable.ReadXml(System.Xml.XmlReader reader)
        {
            this.Clear();
            var serializer = new XmlSerializer(typeof(List<Entry>));
            reader.Read();  // Why is this necessary?
            var list = (List<Entry>)serializer.Deserialize(reader);
            foreach (var entry in list) this.Add(entry.Key, entry.Value);
            reader.ReadEndElement();
        }
        void IXmlSerializable.WriteXml(System.Xml.XmlWriter writer)
        {
            var list = new List<Entry>(this.Count);
            foreach (var entry in this) list.Add(new Entry(entry.Key, entry.Value));
            XmlSerializer serializer = new XmlSerializer(list.GetType());
            serializer.Serialize(writer, list);
        }
    }

    public class SceneModel
    {

        private int _world_unit_size;
        public int world_unit_size { get { return _world_unit_size; } set { _world_unit_size = value; if (this.SettingsChanged != null) this.SettingsChanged(this); } }


        public string project_root;
        private List<SceneBrush> _brushes = new List<SceneBrush>();
        private XmlDictionary<String, BrushStyle> _styles = new XmlDictionary<string, BrushStyle>();

        public List<SceneBrush> brushes { get { return this._brushes; } set { this._brushes = value; } }
        public XmlDictionary<String, BrushStyle> styles { get { return this._styles; } set { this._styles = value; } }

        private List<String> _shaders = new List<string>();

        public delegate void ModelChangedHandler(SceneModel model);
        public event ModelChangedHandler StylesChanged;
        public event ModelChangedHandler BrushesChanged;
        public event ModelChangedHandler SettingsChanged;

        public void moveBrush(SceneBrush target, int delta)
        {
            if (_brushes.Contains(target))
            {
                int idx = _brushes.IndexOf(target);
                if ( 
                    ( delta==1) && (target!= brushes.Last()) ||
                    ( delta==-1) && (target!=brushes.First())
                    
                    )
                {
                    _brushes.RemoveAt(idx);
                    _brushes.Insert(idx + delta, target);
                }
            }

            if (this.BrushesChanged != null) this.BrushesChanged(this);
        }


        public static XmlSerializer getSerializer() {

            return new XmlSerializer(
                                        typeof(SceneModel), 
                                        new Type[] { 
                                                        typeof(SceneBrush), 
                                                        typeof(List<SceneBrush>),
                                                        typeof(Color),
                                                        typeof(BrushStyle),
                                                        typeof(XmlDictionary<String,BrushStyle>)
                                                         }
                                        );
        }

        public void Save()
        {
           
                XmlSerializer x = SceneModel.getSerializer();
                StreamWriter file = new StreamWriter("shadeTool.xml",false,Encoding.Unicode);
                x.Serialize( file, this );
                file.Close();
         
        }

        public static SceneModel Load() {
            try {
                SceneModel tmpModel = new SceneModel();

                XmlSerializer x = SceneModel.getSerializer();
                StreamReader file = new StreamReader("shadeTool.xml", Encoding.Unicode);
                tmpModel = (SceneModel)x.Deserialize( file );
                file.Close();

                if (tmpModel.world_unit_size == 0)
                {
                    tmpModel.world_unit_size = 32;
                }


                return tmpModel;
            }
            catch {
                 Console.WriteLine("error saving");
                return new SceneModel();
            }

            
        }

        public void AddBrush(SceneBrush brush)
        {
            this.brushes.Add(brush);
            if (BrushesChanged != null) {
                BrushesChanged(this);
            }
            
        }

        public void DelBrush(SceneBrush brush)
        {
            this.brushes.Remove(brush);
            if (BrushesChanged != null) {
                BrushesChanged(this);
            }
        }

        public void SetStyle(String key, BrushStyle style)
        {
            this.styles[key] = style;
            if (StylesChanged != null) {
                StylesChanged(this);
            }
        }

        public BrushStyle GetStyle(String key)
        {
            try {
                return this.styles[key];
            }
            catch {
                return new BrushStyle();
            }
        }

        public SceneModel()
        {
            styles.Add("brush_default_style", new BrushStyle());
        }

    }
}
