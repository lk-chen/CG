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

        private void callbackDoNothing(double y, int eventi, int eventj, int slopeIdx, List<UInt32> SLSIdx, int nexti, int nextj) { }

        private void callbackShowMessage(double y, int eventi, int eventj, int slopeIdx, List<UInt32> SLSIdx, int nexti, int nextj) {
            Console.WriteLine("{0},{0}", y, SLSIdx.Count);
        }

        private void btnDraw_Click(object sender, EventArgs e)
        {
            wrapper.Intersection.CSCallbackType cb = callbackDoNothing;
            var intPointTuples = wrapper.Intersection.getIntersection(segmentEndPoints, cb);

            PointF[] intPoints = new PointF[intPointTuples.Count];
            for (int i = 0; i < intPoints.Length; i++)
                intPoints[i] = intPointTuples[i].Item1;

            drawIntPoint(intPoints);
        }

        private void Intersection_MouseDown(object sender, MouseEventArgs e)
        {
            segmentEndPoints.Add(new PointF(e.X, e.Y));
        }

        private void Intersection_MouseUp(object sender, MouseEventArgs e)
        {
            segmentEndPoints.Add(new PointF(e.X, e.Y));
            using (var g = CreateGraphics())
            {
                g.DrawLine(new Pen(Color.Black),
                    segmentEndPoints[segmentEndPoints.Count - 2],
                    segmentEndPoints[segmentEndPoints.Count - 1]);
            }
        }
    }
}
