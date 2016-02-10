using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using wrapper;

namespace cgdemo
{
    public partial class frmMain : Form
    {
        private List<Point> points;
        private Color pointColor = Color.Red;
        private Color lineColor = Color.Black;

        public frmMain()
        {
            InitializeComponent();
            this.frmMain_Resize(null, null);
            points = new List<Point>();
        }

        private void btnDraw_Click(object sender, EventArgs e)
        {
            double[] X = new double[points.Count];
            double[] Y = new double[points.Count];
            for (int i = 0; i < points.Count; i++)
            {
                X[i] = points[i].X;
                Y[i] = points[i].Y;
            }

            var ch = CH.incremental(X, Y);

            var h = ch.Length;
            if (h <= 0) return;
            var hull = new Point[h+1];
            for (int i = 0; i < h; i++)
                hull[i] = points[(int)ch[i]];
            hull[h] = hull[0];
            var g = this.CreateGraphics();
            var pen = new Pen(lineColor);
            g.DrawLines(pen, hull);
            g.Dispose();
            pen.Dispose();
        }

        private void frmMain_Resize(object sender, EventArgs e)
        {
            this.btnDraw.Left = this.Width - this.btnDraw.Width - 30;
            this.btnDraw.Top = 12;
        }

        private void frmMain_MouseUp(object sender, MouseEventArgs e)
        {
            var p = new Point(e.X, e.Y);
            points.Add(p);

            var g = this.CreateGraphics();
            var brush = new SolidBrush(pointColor);
            int diameter = 8;
            g.FillEllipse(brush, p.X - diameter/2, p.Y - diameter/2, diameter, diameter);
            g.Dispose();
            brush.Dispose();
        }
    }
}
