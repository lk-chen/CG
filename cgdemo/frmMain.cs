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
            for (int i = 0; i < btns.Length; i++)
            {
                btns[i] = new Button();
                btns[i].Size= new Size(btnWidth, btnHeight);
                btns[i].Left = (ClientRectangle.Width - 3 * btnWidth) / 2
                    - margin + (i % 3) * (btnWidth + margin);
                btns[i].Top = (ClientRectangle.Height - 3 * btnHeight) / 2
                    - margin + (i / 3) * (btnHeight + margin);
                btns[i].Visible = false;
                
                Controls.Add(btns[i]);
            }
            
            btns[0].Text = "Convex Hull";
            btns[0].Click += delegate (object _sender, EventArgs _e)
            {
                (new CH()).Show();
            };
            btns[0].Visible = true;

            btns[1].Text = "Intersection";
            btns[1].Click += delegate (object _sender, EventArgs _e)
            {
                (new Intersection()).Show();
            };
            btns[1].Visible = true;

            btns[2].Text = "Triangulation";
            btns[2].Click += delegate (object _sender, EventArgs _e)
            {
                (new Triangulation()).Show();
            };
            btns[2].Visible = true;
        }
    }
}
