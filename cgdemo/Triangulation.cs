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
    public partial class Triangulation : Form
    {
        private const int margin = 12;
        private const int btnWidth = 80;
        private const int btnHeight = 40;

        public Triangulation()
        {
            InitializeComponent();
            setLayout();
        }

        /// <summary>
        /// Set static layout
        /// </summary>
        private void setLayout()
        {
            foreach (Control b in this.Controls)
            {
                if (b is Button)
                {
                    b.Height = btnHeight;
                    b.Width = btnWidth;
                }
            }
            btnRand.Top = margin;
            btnRand.Left = margin;
        }

        private void btnRand_Click(object sender, EventArgs e)
        {
            Random r = new Random();
            var sz = r.Next(5, 10);
            int gap = this.ClientSize.Height / sz;
            int width = this.ClientSize.Width;
            LinkedList<PointF> points = new LinkedList<PointF>();

            for (int i = 0; i < sz; i++)
            {
                bool putOnLeft = r.Next() % 2 == 0;
                var widthOffset = r.NextDouble() * width / 4;
                PointF p = new PointF(
                    (float)(putOnLeft ? width / 2 - widthOffset : width / 2 + widthOffset),
                    (float)(r.NextDouble() * gap + gap * i));
                if (putOnLeft)
                    points.AddFirst(p);
                else
                    points.AddLast(p);
            }

            //var res = wrapper.Triangulation.triangulation(points.ToList());
            using (var g = this.CreateGraphics())
            {
                using (var b = (Brush)Brushes.Black)
                {
                    foreach (var p in points)
                    {
                        g.FillEllipse(b, p.X, p.Y, 6, 6);
                    }
                    g.DrawPolygon(new Pen(Color.Blue), points.ToArray());
                }
            }
        }
    }
}
