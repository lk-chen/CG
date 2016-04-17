using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace cgdemo
{
    public partial class Intersection : Form
    {
        private List<PointF> segmentEndPoints = new List<PointF>();
        private int pointDiameter = 6;
        private Color pointColor = Color.Red;
        private Color segmentColor = Color.DarkGray;
        private Color sweepLineColor = Color.OrangeRed;
        private Color eventSegColor = Color.Blue;
        private Color tagColor = Color.Purple;
        private BufferedGraphics[] bufferLayers;
        private const int margin = 12;
        private const int btnWidth = 80;
        private const int btnHeight = 40;
        private enum BufferLayerType
        {
            TBackground,
            TSegments,
            TIntersections,
            TAnimation,
        };

        public Intersection()
        {
            InitializeComponent();
            setLayout();
            initBufferLayers();
        }

        /// <summary>
        /// Set static layout, then call dynamic layout set.
        /// </summary>
        private void setLayout()
        {
            foreach (Control b in this.Controls)
                if (b is Button)
                {
                    b.Width = btnWidth;
                    b.Height = btnHeight;
                }
            btnDraw.Left = margin;
            btnDraw.Top = margin;

            btnLoad.Left = btnDraw.Right + margin;
            btnLoad.Top = margin;
            btnSave.Left = btnLoad.Right + margin;
            btnSave.Top = margin;
            btnNext.Left = btnSave.Right + margin;
            btnNext.Top = margin;
            btnNext.Visible = false;
            btnNext.Tag = false;
            lblAnimation.Visible = ckbAnimation.Checked;
            lblAnimation.TextAlign = ContentAlignment.MiddleRight;

            setDynamicLayout();
        }

        /// <summary>
        /// Set dynamic layout
        /// </summary>
        private void setDynamicLayout()
        {
            lblCoord.Left = margin;
            lblCoord.Top = ClientSize.Height - lblCoord.Height - margin;

            ckbAnimation.Top = margin;
            ckbAnimation.Left = ClientSize.Width - ckbAnimation.Width - margin;
            lblAnimation.Left = ClientSize.Width - lblAnimation.Width - margin;
            lblAnimation.Top = ckbAnimation.Bottom + margin;
        }

        /// <summary>
        /// Initialize each buffer layer.
        /// </summary>
        private void initBufferLayers()
        {
            bufferLayers = new BufferedGraphics[Enum.GetNames(typeof(BufferLayerType)).Length];

            var curContext = BufferedGraphicsManager.Current;
            for (int i = 0; i < bufferLayers.Length; i++)
                bufferLayers[i] = curContext.Allocate(
                    CreateGraphics(), DisplayRectangle);
            bufferLayers[(int)BufferLayerType.TBackground].Graphics.Clear(BackColor);

            reDrawSegments();
        }

        /// <summary>
        /// Draw segments on buffer.
        /// </summary>
        private void reDrawSegments()
        {
            bufferLayers[(int)BufferLayerType.TBackground].Render(
                bufferLayers[(int)BufferLayerType.TSegments].Graphics);

            using (var pen = new Pen(segmentColor))
                using(var tagBrush = new SolidBrush(tagColor))
            {
                for (int i = 0; i < segmentEndPoints.Count; i += 2)
                {
                    var p1 = segmentEndPoints[i];
                    var p2 = segmentEndPoints[i + 1];
                    bufferLayers[(int)BufferLayerType.TSegments].Graphics
                        .DrawLine(pen, p1, p2);
                    bufferLayers[(int)BufferLayerType.TSegments].Graphics
                        .DrawString((i / 2).ToString(), SystemFonts.DefaultFont,
                        tagBrush, new PointF((p1.X + p2.X) / 2, (p1.Y + p2.Y) / 2));
                }
            }

            drawIntPoint(new PointF[0]);
        }

        /// <summary>
        /// Draw intersection points
        /// </summary>
        /// <param name="intPoints"> Array of intersection points </param>
        private void drawIntPoint(PointF[] intPoints)
        {
            bufferLayers[(int)BufferLayerType.TSegments].Render
                (bufferLayers[(int)BufferLayerType.TIntersections].Graphics);

            using(var brush = new SolidBrush(pointColor))
            {
                foreach (var point in intPoints)
                    bufferLayers[(int)BufferLayerType.TIntersections].Graphics
                        .FillEllipse(brush, point.X - pointDiameter / 2, point.Y - pointDiameter / 2,
                    pointDiameter, pointDiameter);
            }

            bufferLayers[(int)BufferLayerType.TIntersections].Render();
        }

        /// <summary>
        /// Call back function when user does not want animation.
        /// </summary>
        /// <param name="y"></param>
        /// <param name="eventi"></param>
        /// <param name="eventj"></param>
        /// <param name="slopeIdx"></param>
        /// <param name="SLSIdx"></param>
        /// <param name="nexti"></param>
        /// <param name="nextj"></param>
        private void callbackDoNothing(double y, int eventi, int eventj, int slopeIdx, List<UInt32> SLSIdx, int nexti, int nextj) { }

        /// <summary>
        /// Call back function to show animation.
        /// </summary>
        /// <param name="y"></param>
        /// <param name="eventi"></param>
        /// <param name="eventj"></param>
        /// <param name="slopeIdx"></param>
        /// <param name="SLSIdx"></param>
        /// <param name="nexti"></param>
        /// <param name="nextj"></param>
        private void callbackShowAnimation(double y, int eventi, int eventj, int slopeIdx, List<UInt32> SLSIdx, int nexti, int nextj)
        {
            bufferLayers[(int)BufferLayerType.TSegments].Render(
                bufferLayers[(int)BufferLayerType.TAnimation].Graphics);

            using (var dashPen = new Pen(sweepLineColor))
            {
                var g = bufferLayers[(int)BufferLayerType.TAnimation].Graphics;
                dashPen.DashStyle = DashStyle.Dash;
                g.DrawLine(dashPen, new PointF(0, (float)y), new PointF(ClientSize.Width, (float)y));
            }
            using (var pen = new Pen(eventSegColor))
            {
                var g = bufferLayers[(int)BufferLayerType.TAnimation].Graphics;
                if (eventi >= 0) g.DrawLine(pen, segmentEndPoints[eventi * 2], segmentEndPoints[eventi * 2 + 1]);
                if (eventj >= 0) g.DrawLine(pen, segmentEndPoints[eventj * 2], segmentEndPoints[eventj * 2 + 1]);
            }
            
            lblAnimation.Text = "slope: " + slopeIdx.ToString() + '\n';
            lblAnimation.Text += "SLS: " + string.Join(", ", SLSIdx) + '\n';
            lblAnimation.Text += "next event: ";
            if (nexti < 0)
                if (nextj < 0)
                    lblAnimation.Text += "[end]";
                else
                    lblAnimation.Text += "pop " + nextj.ToString();
            else
            {
                if (nextj < 0)
                    lblAnimation.Text += "push " + nexti.ToString();
                else
                    lblAnimation.Text += "intersect (" + nexti.ToString() + ',' + nextj.ToString() + ')';
            }

            bufferLayers[(int)BufferLayerType.TAnimation].Render();

            MessageBox.Show("");
        }

        private void btnDraw_Click(object sender, EventArgs e)
        {
            List<Tuple<PointF, uint, uint>> intPointTuples;
            if (!ckbAnimation.Checked)
                intPointTuples = wrapper.Intersection.getIntersection(segmentEndPoints, callbackDoNothing);
            else
                intPointTuples = wrapper.Intersection.getIntersection(segmentEndPoints, callbackShowAnimation);

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
            var p1 = segmentEndPoints.Last();
            segmentEndPoints.Add(new PointF(e.X, e.Y));
            var p2 = segmentEndPoints.Last();
            var tag = segmentEndPoints.Count / 2 - 1;

            var g = bufferLayers[(int)BufferLayerType.TSegments].Graphics;
            g.DrawLine(new Pen(segmentColor), p2, p1);
            g.DrawString(tag.ToString(), SystemFonts.DefaultFont,
            new SolidBrush(tagColor), new PointF((p1.X + p2.X) / 2, (p1.Y + p2.Y) / 2));

            drawIntPoint(new PointF[0]);
        }

        private void Intersection_Resize(object sender, EventArgs e)
        {
            setDynamicLayout();
        }

        private void Intersection_MouseMove(object sender, MouseEventArgs e)
        {
            lblCoord.Text = '(' + e.X.ToString() + ',' + e.Y.ToString() + ')';
        }

        private void ckbAnimation_CheckedChanged(object sender, EventArgs e)
        {
            btnNext.Visible = ckbAnimation.Checked;
            lblAnimation.Visible = ckbAnimation.Checked;
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            btnNext.Tag = true;
        }

        private void lblAnimation_Resize(object sender, EventArgs e)
        {
            lblAnimation.Left = ClientRectangle.Width - lblAnimation.Width - margin;
        }
    }
}
