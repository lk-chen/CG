namespace cgdemo
{
    partial class Triangulation
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
            this.btnRand = new System.Windows.Forms.Button();
            this.btnDraw = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnRand
            // 
            this.btnRand.Location = new System.Drawing.Point(81, 61);
            this.btnRand.Name = "btnRand";
            this.btnRand.Size = new System.Drawing.Size(111, 69);
            this.btnRand.TabIndex = 0;
            this.btnRand.Text = "Random";
            this.btnRand.UseVisualStyleBackColor = true;
            this.btnRand.Click += new System.EventHandler(this.btnRand_Click);
            // 
            // btnDraw
            // 
            this.btnDraw.Location = new System.Drawing.Point(263, 72);
            this.btnDraw.Name = "btnDraw";
            this.btnDraw.Size = new System.Drawing.Size(110, 58);
            this.btnDraw.TabIndex = 1;
            this.btnDraw.Text = "Draw";
            this.btnDraw.UseVisualStyleBackColor = true;
            this.btnDraw.Click += new System.EventHandler(this.btnDraw_Click);
            // 
            // Triangulation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1174, 604);
            this.Controls.Add(this.btnDraw);
            this.Controls.Add(this.btnRand);
            this.MinimumSize = new System.Drawing.Size(1200, 675);
            this.Name = "Triangulation";
            this.Text = "Triangulation";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnRand;
        private System.Windows.Forms.Button btnDraw;
    }
}