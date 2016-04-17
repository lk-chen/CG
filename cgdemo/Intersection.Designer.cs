namespace cgdemo
{
    partial class Intersection
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
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.lblCoord = new System.Windows.Forms.Label();
            this.ckbAnimation = new System.Windows.Forms.CheckBox();
            this.btnNext = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnDraw
            // 
            this.btnDraw.Location = new System.Drawing.Point(62, 40);
            this.btnDraw.Name = "btnDraw";
            this.btnDraw.Size = new System.Drawing.Size(120, 65);
            this.btnDraw.TabIndex = 0;
            this.btnDraw.Text = "Draw";
            this.btnDraw.UseVisualStyleBackColor = true;
            this.btnDraw.Click += new System.EventHandler(this.btnDraw_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(250, 40);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(131, 73);
            this.btnLoad.TabIndex = 1;
            this.btnLoad.Text = "Load...";
            this.btnLoad.UseVisualStyleBackColor = true;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(469, 40);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(141, 96);
            this.btnSave.TabIndex = 2;
            this.btnSave.Text = "Save...";
            this.btnSave.UseVisualStyleBackColor = true;
            // 
            // lblCoord
            // 
            this.lblCoord.AutoSize = true;
            this.lblCoord.Location = new System.Drawing.Point(80, 567);
            this.lblCoord.Name = "lblCoord";
            this.lblCoord.Size = new System.Drawing.Size(70, 25);
            this.lblCoord.TabIndex = 3;
            this.lblCoord.Text = "Coord";
            // 
            // ckbAnimation
            // 
            this.ckbAnimation.AutoSize = true;
            this.ckbAnimation.Location = new System.Drawing.Point(852, 40);
            this.ckbAnimation.Name = "ckbAnimation";
            this.ckbAnimation.Size = new System.Drawing.Size(137, 29);
            this.ckbAnimation.TabIndex = 4;
            this.ckbAnimation.Text = "animation";
            this.ckbAnimation.UseVisualStyleBackColor = true;
            this.ckbAnimation.CheckedChanged += new System.EventHandler(this.ckbAnimation_CheckedChanged);
            // 
            // btnNext
            // 
            this.btnNext.Location = new System.Drawing.Point(667, 28);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(90, 96);
            this.btnNext.TabIndex = 5;
            this.btnNext.Text = "next";
            this.btnNext.UseVisualStyleBackColor = true;
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // Intersection
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1340, 697);
            this.Controls.Add(this.btnNext);
            this.Controls.Add(this.ckbAnimation);
            this.Controls.Add(this.lblCoord);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnDraw);
            this.MinimumSize = new System.Drawing.Size(1200, 675);
            this.Name = "Intersection";
            this.Text = "Intersection";
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Intersection_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Intersection_MouseMove);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Intersection_MouseUp);
            this.Resize += new System.EventHandler(this.Intersection_Resize);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnDraw;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Label lblCoord;
        private System.Windows.Forms.CheckBox ckbAnimation;
        private System.Windows.Forms.Button btnNext;
    }
}