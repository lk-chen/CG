using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace cgdemo
{
    public partial class Intersection : Form
    {
        private List<PointF> segmentEndPoints = new List<PointF>();
        private int pointDiameter = 8;
        private float segWidth = 1.0f;
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

        private class Animation
        {
            private List<double> ys;
            private List<List<UInt32>> eventIndices;
            private List<List<UInt32>> SLSIndices;
            public int step;
            public int frameNum { get { return _frameNum; } }
            private int _frameNum;
            private Intersection form;
            public PointF[] intPoints;

            public Animation (Intersection fm) {
                step = _frameNum = 0;
                form = fm;
                ys = new List<double>();
                eventIndices = new List<List<uint>>();
                SLSIndices = new List<List<uint>>();
            }

            public void push(double y, List<UInt32> eventIdx, List<UInt32> SLSIdx)
            {
                _frameNum++;
                ys.Add(y);
                eventIndices.Add(eventIdx);
                SLSIndices.Add(SLSIdx);
            }

            public void flush()
            {
                if (end()) return;
                form.bufferLayers[(int)BufferLayerType.TSegments].Render(
                    form.bufferLayers[(int)BufferLayerType.TAnimation].Graphics);

                using (var dashPen = new Pen(form.sweepLineColor))
                {
                    var g = form.bufferLayers[(int)BufferLayerType.TAnimation].Graphics;
                    dashPen.DashStyle = DashStyle.Dash;
                    g.DrawLine(dashPen, new PointF(0, (float)ys[step]), new PointF(form.ClientSize.Width, (float)ys[step]));
                }
                using (var pen = new Pen(form.eventSegColor, form.segWidth + 1.0f))
                {
                    var g = form.bufferLayers[(int)BufferLayerType.TAnimation].Graphics;
                    foreach (int i in eventIndices[step])
                        g.DrawLine(pen, form.segmentEndPoints[i * 2], form.segmentEndPoints[i * 2 + 1]);
                }

                form.lblAnimation.Text = "SLS: " + string.Join(", ", SLSIndices[step]) + '\n';
                
                form.bufferLayers[(int)BufferLayerType.TAnimation].Render();
            }
            
            public bool end() { return step >= frameNum || step < 0; }
        }

        Animation animation;

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
            btnPrev.Left = btnSave.Right + margin;
            btnPrev.Top = margin;
            btnNext.Left = btnPrev.Right + margin;
            btnNext.Top = margin;
            btnNext.Visible=btnPrev.Visible = false;
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

            ckbPolygon.Top = margin;
            ckbPolygon.Left = ClientSize.Width - ckbPolygon.Width - margin;
            ckbAnimation.Top = ckbPolygon.Bottom + margin;
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

            using (var pen = new Pen(segmentColor, segWidth))
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
        /// Call back function to show animation.
        /// </summary>
        /// <param name="y"></param>
        /// <param name="eventi"></param>
        /// <param name="eventj"></param>
        /// <param name="slopeIdx"></param>
        /// <param name="SLSIdx"></param>
        /// <param name="nexti"></param>
        /// <param name="nextj"></param>
        private void callbackShowAnimation(double y, List<UInt32> eventIdx, List<UInt32> SLSIdx)
        {
            animation.push(y, eventIdx, SLSIdx);
        }

        private void btnDraw_Click(object sender, EventArgs e)
        {
            animation = new Animation(this);
            btnNext.Enabled = btnPrev.Enabled = true;
            var intPointTuples = wrapper.Intersection.getIntersection(segmentEndPoints, callbackShowAnimation);

            List<PointF> intPoints = new List<PointF>();
            foreach (var tuple in intPointTuples)
                intPoints.Add(tuple.Item1);

            if (ckbPolygon.Checked)
            {
                intPoints = new List<PointF>();
                foreach (var tuple in intPointTuples)
                    if (Math.Abs((int)tuple.Item2 - (int)tuple.Item3) == 1)
                        continue;
                    else if (Math.Abs((int)tuple.Item2 - (int)tuple.Item3)
                            != segmentEndPoints.Count / 2 - 1)
                        intPoints.Add(tuple.Item1);
            }

            animation.intPoints = intPoints.ToArray();
            drawIntPoint(animation.intPoints);
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
            initBufferLayers();
        }

        private void Intersection_MouseMove(object sender, MouseEventArgs e)
        {
            lblCoord.Text = '(' + e.X.ToString() + ',' + e.Y.ToString() + ')';
        }

        private void ckbAnimation_CheckedChanged(object sender, EventArgs e)
        {
            btnPrev.Visible = btnNext.Visible = ckbAnimation.Checked;
            lblAnimation.Visible = ckbAnimation.Checked;
        }

        private void lblAnimation_Resize(object sender, EventArgs e)
        {
            lblAnimation.Left = ClientRectangle.Width - lblAnimation.Width - margin;
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Text Files (.txt)|*.txt";
            var openFileOK = openFileDialog.ShowDialog();

            if (openFileOK == DialogResult.OK)
            {
                var fileStream = openFileDialog.OpenFile();
                StreamReader reader = null;
                var temp = new List<PointF>();
                try
                {
                    reader = new StreamReader(fileStream);
                    var line = reader.ReadLine();
                    var N = uint.Parse(line);
                    Regex spaceSpliter = new Regex(@"\s+");
                    for (int i = 0; i < N; i++)
                    {
                        line = reader.ReadLine();
                        var tokens = spaceSpliter.Split(line);
                        var x = float.Parse(tokens[0]);
                        var y = float.Parse(tokens[1]);
                        temp.Add(new PointF(x, y));
                    }

                    segmentEndPoints = temp;
                    reDrawSegments();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
                finally
                {
                    if (reader != null)
                        reader.Close();
                    fileStream.Close();
                }
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Text Files (.txt)|*.txt";
            var openFileOK = openFileDialog.ShowDialog();

            if (openFileOK == DialogResult.OK)
            {
                StreamWriter writer = null;
                try
                {
                    writer = new StreamWriter(openFileDialog.FileName, false);
                    writer.WriteLine(segmentEndPoints.Count);
                    foreach (var point in segmentEndPoints)
                    {
                        writer.WriteLine(point.X.ToString() + ' ' + point.Y.ToString());
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
                finally
                {
                    if (writer != null)
                        writer.Close();
                }
            }
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            if (animation.step < 0) animation.step = 0;
            if (!animation.end())
            {
                animation.flush();
                btnPrev.Enabled = true;
                animation.step++;
            }
            else
            {
                drawIntPoint(animation.intPoints);
                btnNext.Enabled = false;
            }
        }

        private void btnPrev_Click(object sender, EventArgs e)
        {
            animation.step--;
            if (!animation.end())
            {
                animation.flush();
                btnNext.Enabled = true;
            }
            else
            {
                bufferLayers[(int)BufferLayerType.TSegments].Render();
                btnPrev.Enabled = false;
            }
        }
    }
}
