using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace cgdemo
{
    public partial class frmMain : Form
    {
        public frmMain()
        {
            InitializeComponent();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            Width = 400;
            Height = 320;
            MaximumSize = new Size(Width, Height);
            MinimumSize = MaximumSize;
            var margin = 20;
            var btnWidth = 80;
            var btnHeight = 40;

            Button[] btns = new Button[9];

            btns[0] = new Button();
            btns[0].Text = "Convex Hull";
            btns[0].Size = new Size(btnWidth, btnHeight);
            btns[0].Left = (ClientRectangle.Width - 3 * btnWidth) / 2 - margin;
            btns[0].Top = (ClientRectangle.Height - 3 * btnHeight) / 2 - margin;
            btns[0].Click += delegate (object _sender, EventArgs _e)
             {
                 (new CH()).Show();
             };
            btns[0].Visible = true;

            foreach (var btn in btns)
                Controls.Add(btn);
        }
    }
}
