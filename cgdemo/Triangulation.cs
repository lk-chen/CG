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
    public partial class Triangulation : Form
    {
        private Color pointColor = Color.Red;
        private int pointDiameter = 8;
        private Color lineColor = Color.Black;
        private Color triLineColor = Color.Blue;
        private const int margin = 12;
        private const int btnWidth = 80;
        private const int btnHeight = 40;
        private BufferedGraphics[] bufferLayers;
        private List<PointF> vertices;
        private enum BufferLayerType
        {
            TBackground,
            TPolygon,
            TTriangulation,
        };

        public Triangulation()
        {
            InitializeComponent();
            setLayout();
            initBufferLayers();
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

            btnDraw.Top = margin;
            btnDraw.Left = btnRand.Right + margin;
        }

        /// <summary>
        /// Initialize graphics buffer layers.
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

            reDrawPolygon(new List<PointF>());
        }

        private void reDrawPolygon(List<PointF> vertices)
        {
            bufferLayers[(int)BufferLayerType.TBackground]
                .Render(bufferLayers[(int)BufferLayerType.TPolygon].Graphics);

            if (vertices != null && vertices.Count > 0)
                using (var brush = new SolidBrush(pointColor))
                using (var linePen = new Pen(lineColor))
                {
                    foreach (var vertex in vertices)
                        bufferLayers[(int)BufferLayerType.TPolygon].Graphics
                            .FillEllipse(brush, vertex.X - pointDiameter / 2,
                            vertex.Y - pointDiameter / 2, pointDiameter, pointDiameter);
                    bufferLayers[(int)BufferLayerType.TPolygon].Graphics
                        .DrawPolygon(linePen, vertices.ToArray());
                }

            reDrawTriangulation(new List<Tuple<uint, uint>>());
        }

        private void reDrawTriangulation(List<Tuple<uint, uint>> tri)
        {
            bufferLayers[(int)BufferLayerType.TPolygon]
                .Render(bufferLayers[(int)BufferLayerType.TTriangulation].Graphics);

            using (var pen = new Pen(triLineColor))
            {
                pen.DashStyle = DashStyle.Dash;
                foreach (var triLine in tri)
                {
                    bufferLayers[(int)BufferLayerType.TTriangulation].Graphics
                        .DrawLine(pen, vertices[(int)triLine.Item1], vertices[(int)triLine.Item2]);
                }
            }

            bufferLayers[(int)BufferLayerType.TTriangulation].Render();
        }

        private void btnRand_Click(object sender, EventArgs e)
        {
            Random r = new Random();
            var sz = r.Next(8, 12);
            int gap = this.ClientSize.Height / sz;
            int width = this.ClientSize.Width;
            LinkedList<PointF> points = new LinkedList<PointF>();

            points.AddFirst(new PointF(width / 2,
                (float)r.NextDouble() * gap));
            for (int i = 1; i < sz - 1; i++)
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
            points.AddFirst(new PointF(width / 2,
                gap * sz - (float)r.NextDouble() * gap));

            vertices = points.ToList();
            reDrawPolygon(vertices);
        }

        private void btnDraw_Click(object sender, EventArgs e)
        {
            var res = wrapper.Triangulation.triangulation(vertices);
            reDrawTriangulation(res);
        }
    }
}
