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
    public partial class Intersection : Form
    {
        private List<PointF> segmentEndPoints = new List<PointF>();

        public Intersection()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Draw intersection points
        /// </summary>
        /// <param name="intPoints"> Array of intersection points </param>
        private void drawIntPoint(PointF[] intPoints)
        {
            var g = CreateGraphics();
            var brush = new SolidBrush(Color.Red);
            var diameter = 8;
            foreach (var point in intPoints)
            {
                g.FillEllipse(brush, point.X - diameter / 2, point.Y - diameter / 2,
                    diameter, diameter);
            }
            brush.Dispose();
            g.Dispose();
        }

        private void btnDraw_Click(object sender, EventArgs e)
        {
            double[] X = new double[segmentEndPoints.Count];
            double[] Y = new double[segmentEndPoints.Count];
            for (int i = 0;i < segmentEndPoints.Count; i++)
            {
                X[i] = segmentEndPoints[i].X;
                Y[i] = segmentEndPoints[i].Y;
            }

            var intPointCoord = wrapper.Intersection.getIntersection(X, Y);

            PointF[] intPoints = new PointF[intPointCoord.Length / 2];
            for (int i = 0;i < intPointCoord.Length / 2; i++)
            {
                intPoints[i].X = (float)intPointCoord[2 * i];
                intPoints[i].Y = (float)intPointCoord[2 * i + 1];
            }

            drawIntPoint(intPoints);
        }

        private void Intersection_MouseDown(object sender, MouseEventArgs e)
        {
            segmentEndPoints.Add(new PointF(e.X, e.Y));
        }

        private void Intersection_MouseUp(object sender, MouseEventArgs e)
        {
            segmentEndPoints.Add(new PointF(e.X, e.Y));
            var g = CreateGraphics();
            g.DrawLine(new Pen(Color.Black),
                segmentEndPoints[segmentEndPoints.Count - 2],
                segmentEndPoints[segmentEndPoints.Count - 1]);
            g.Dispose();
        }
    }
}
