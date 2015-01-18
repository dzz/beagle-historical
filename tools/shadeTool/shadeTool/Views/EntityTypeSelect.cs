using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using shadeTool.Models;
using System.IO;

namespace shadeTool.Views
{
    public partial class EntityTypeSelect : ModelControllerView
    {

        public string value { get; set; }
        public EntityTypeSelect()
        {
            InitializeComponent();
        }

        protected override void synchRootModel(SceneModel model)
        {
            string lookup = model.project_root + "script/";

            string[] files = Directory.GetFiles(lookup, "*.default");

            List<string> parsedFiles = new List<string>();

            foreach (string pf in files)
            {
                parsedFiles.Add(pf.Replace(lookup, "").Replace(".default",""));
            }
            comboBox1.Items.AddRange(parsedFiles.ToArray());

        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.value = comboBox1.Text;

            this.Close();
        }
    }
}
