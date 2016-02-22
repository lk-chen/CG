﻿using System;
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
        private List<PointF> points;
        private Color pointColor = Color.Red;
        private Color lineColor = Color.Black;
        private const int margin = 12;
        private const int btnWidth = 80;
        private const int btnHeight = 40;

        public frmMain()
        {
            InitializeComponent();
            setLayout();
            points = new List<PointF>();
        }

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

            lblPoints.Left = btnSave.Right + margin;
            lblPoints.Top = margin;
            lblPoints.Text = "";
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
            var hull = new PointF[h+1];
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
        }

        private void frmMain_MouseUp(object sender, MouseEventArgs e)
        {
            var p = new PointF(e.X, e.Y);
            points.Add(p);

            var g = this.CreateGraphics();
            var brush = new SolidBrush(pointColor);
            int diameter = 8;
            g.FillEllipse(brush, p.X - diameter/2, p.Y - diameter/2, diameter, diameter);
            g.Dispose();
            brush.Dispose();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {

        }
    }
}
