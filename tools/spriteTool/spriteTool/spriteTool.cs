using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace spriteTool
{
    public partial class spriteTool : Form
    {
        public spriteTool()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            newStrip ns = new newStrip();
            ns.ShowDialog();
        }
    }
}
