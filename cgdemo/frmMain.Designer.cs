﻿namespace cgdemo
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnDraw = new System.Windows.Forms.Button();
            this.txtX = new System.Windows.Forms.TextBox();
            this.txtY = new System.Windows.Forms.TextBox();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.lblPoints = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnDraw
            // 
            this.btnDraw.Location = new System.Drawing.Point(36, 12);
            this.btnDraw.Name = "btnDraw";
            this.btnDraw.Size = new System.Drawing.Size(166, 68);
            this.btnDraw.TabIndex = 0;
            this.btnDraw.Text = "Draw";
            this.btnDraw.UseVisualStyleBackColor = true;
            this.btnDraw.Click += new System.EventHandler(this.btnDraw_Click);
            // 
            // txtX
            // 
            this.txtX.Location = new System.Drawing.Point(284, 31);
            this.txtX.Name = "txtX";
            this.txtX.Size = new System.Drawing.Size(100, 31);
            this.txtX.TabIndex = 1;
            // 
            // txtY
            // 
            this.txtY.Location = new System.Drawing.Point(422, 31);
            this.txtY.Name = "txtY";
            this.txtY.Size = new System.Drawing.Size(100, 31);
            this.txtY.TabIndex = 2;
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(588, 12);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(161, 72);
            this.btnAdd.TabIndex = 3;
            this.btnAdd.Text = "Add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(800, 14);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(131, 68);
            this.btnLoad.TabIndex = 4;
            this.btnLoad.Text = "Load..";
            this.btnLoad.UseVisualStyleBackColor = true;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(970, 20);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(148, 53);
            this.btnSave.TabIndex = 5;
            this.btnSave.Text = "Save..";
            this.btnSave.UseVisualStyleBackColor = true;
            // 
            // lblPoints
            // 
            this.lblPoints.AutoSize = true;
            this.lblPoints.Location = new System.Drawing.Point(1180, 31);
            this.lblPoints.Name = "lblPoints";
            this.lblPoints.Size = new System.Drawing.Size(94, 25);
            this.lblPoints.TabIndex = 6;
            this.lblPoints.Text = "lblPoints";
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1340, 736);
            this.Controls.Add(this.lblPoints);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.txtY);
            this.Controls.Add(this.txtX);
            this.Controls.Add(this.btnDraw);
            this.MinimumSize = new System.Drawing.Size(1200, 675);
            this.Name = "frmMain";
            this.Text = "CG_LinkunChen";
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.frmMain_MouseUp);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnDraw;
        private System.Windows.Forms.TextBox txtX;
        private System.Windows.Forms.TextBox txtY;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Label lblPoints;
    }
}

