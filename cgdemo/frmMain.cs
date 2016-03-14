using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

using wrapper;

namespace cgdemo
{
    public partial class frmMain : Form
    {
        private List<PointF> points = new List<PointF>();
        private Color pointColor = Color.Red;
        private int pointDiameter = 8;
        private Color lineColor = Color.Black;
        private BufferedGraphics[] bufferLayers;
        private const int margin = 12;
        private const int btnWidth = 80;
        private const int btnHeight = 40;
        private enum BufferLayerType
        {
            TBackground,
            TPoints,
            THull,
        };

        public frmMain()
        {
            InitializeComponent();
            setLayout();
            initBufferLayers();
        }

        /// <summary>
        /// Initialize graphics buffer layers, set size and background color
        /// to be the same as system default.
        /// </summary>
        private void initBufferLayers()
        {
            bufferLayers = new BufferedGraphics[Enum.GetNames(typeof(BufferLayerType)).Length];

            var curContext = BufferedGraphicsManager.Current;
            for (int i = 0; i < bufferLayers.Length; i++)
            {
                bufferLayers[i] = curContext.Allocate(
                    CreateGraphics(), DisplayRectangle);
            }
            bufferLayers[(int)BufferLayerType.TBackground].Graphics.Clear(BackColor);

            reDrawPoints();
        }

        /// <summary>
        /// Redraw all the points, used when some points are removed.
        /// Then clear all the hull segments.
        /// </summary>
        private void reDrawPoints()
        {
            bufferLayers[(int)BufferLayerType.TBackground]
                .Render(bufferLayers[(int)BufferLayerType.TPoints].Graphics);

            var brush = new SolidBrush(pointColor);
            foreach (var p in points)
            {
                bufferLayers[(int)BufferLayerType.TPoints].Graphics
                    .FillEllipse(brush, p.X - pointDiameter / 2,
                    p.Y - pointDiameter / 2, pointDiameter, pointDiameter);
            }
            brush.Dispose();

            bufferLayers[(int)BufferLayerType.TPoints]
                .Render(bufferLayers[(int)BufferLayerType.THull].Graphics);
            drawHull(new uint[0]);
        }

        /// <summary>
        /// Draw another point on point layer.
        /// Then clear all the hull segments.
        /// </summary>
        private void drawPoint(PointF p)
        {
            var brush = new SolidBrush(pointColor);
            bufferLayers[(int)BufferLayerType.TPoints].Graphics
                .FillEllipse(brush, p.X - pointDiameter / 2,
                p.Y - pointDiameter / 2, pointDiameter, pointDiameter);
            brush.Dispose();

            bufferLayers[(int)BufferLayerType.TPoints]
                .Render(bufferLayers[(int)BufferLayerType.THull].Graphics);
            drawHull(new uint[0]);
        }

        /// <summary>
        /// Draw hull on the window form according to the given vertex index.
        /// </summary>
        /// <param name="ch">
        /// Indices of all extreme points. </param>
        private void drawHull(uint[] ch)
        {
            var h = ch.Length;
            if (h <= 0)
            {
                bufferLayers[(int)BufferLayerType.THull]
                    .Render(CreateGraphics());
                return;
            }
            var hull = new PointF[h + 1];
            for (int i = 0; i < h; i++)
                hull[i] = points[(int)ch[i]];
            hull[h] = hull[0];

            var pen = new Pen(lineColor);
            bufferLayers[(int)BufferLayerType.THull].Graphics
                .DrawLines(pen, hull);
            bufferLayers[(int)BufferLayerType.THull].Render();
            pen.Dispose();
        }

        /// <summary>
        /// Set static controls' layout, then set dynamic controls' layout.
        /// </summary>
        private void setLayout()
        {
            foreach (Control b in this.Controls)
            {
                if (b is Button)
                {
                    b.Width = btnWidth;
                    b.Height = btnHeight;
                }
            }
            btnDraw.Left = margin;
            btnDraw.Top = margin;

            txtX.Left = btnDraw.Right + margin;
            txtX.Top = margin + (btnDraw.Height - txtX.Height) / 2;
            txtY.Left = txtX.Right + margin;
            txtY.Top = margin + (btnDraw.Height - txtY.Height) / 2;

            btnAdd.Left = txtY.Right + margin;
            btnAdd.Top = margin;
            btnLoad.Left = btnAdd.Right + margin;
            btnLoad.Top = margin;
            btnSave.Left = btnLoad.Right + margin;
            btnSave.Top = margin;

            setDynamicLayout();
        }

        /// <summary>
        /// Set dynamic controls' layout.
        /// </summary>
        private void setDynamicLayout()
        {
            lblCoord.Left = margin;
            lblCoord.Top = ClientSize.Height - lblCoord.Height - margin;
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

            var ch = CH.getCH(X, Y);

            drawHull(ch);
        }

        private void frmMain_Resize(object sender, EventArgs e)
        {
            setDynamicLayout();
            initBufferLayers();
            btnDraw_Click(sender, e);
        }

        /// <summary>
        /// Add one point into array, and draw it.
        /// </summary>
        /// <param name="p"> Point to be added </param>
        private void addPoint(PointF p)
        {
            points.Add(p);
            drawPoint(p);
        }

        private void frmMain_MouseUp(object sender, MouseEventArgs e)
        {
            addPoint(new PointF(e.X, e.Y));
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            Regex numRegex = new Regex(@"^\d+(\.\d*)?$");
            if (numRegex.IsMatch(txtX.Text) && numRegex.IsMatch(txtY.Text))
            {
                var x = float.Parse(txtX.Text);
                var y = float.Parse(txtY.Text);

                addPoint(new PointF(x, y));
            }
            else
            {
                MessageBox.Show("Please input numeric number.");
            }
        }

        private void frmMain_MouseMove(object sender, MouseEventArgs e)
        {
            lblCoord.Text = '(' + e.X.ToString() + ',' + e.Y.ToString() + ')';
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

                    points = temp;
                    reDrawPoints();
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
                finally
                {
                    if (reader!=null)
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
                    writer.WriteLine(points.Count);
                    foreach (var point in points)
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
    }
}
