namespace PathViewer
{
    partial class RobotParametersForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.m_ok = new System.Windows.Forms.Button();
            this.m_cancel = new System.Windows.Forms.Button();
            this.m_width_text = new System.Windows.Forms.TextBox();
            this.m_length_text = new System.Windows.Forms.TextBox();
            this.m_velocity_text = new System.Windows.Forms.TextBox();
            this.m_accel_text = new System.Windows.Forms.TextBox();
            this.m_jerk_text = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(82, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(98, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "Robot Width";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(76, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(104, 20);
            this.label2.TabIndex = 1;
            this.label2.Text = "Robot Height";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(45, 80);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(135, 20);
            this.label3.TabIndex = 2;
            this.label3.Text = "Maximum Velocity";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 112);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(168, 20);
            this.label4.TabIndex = 3;
            this.label4.Text = "Maximum Acceleration";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(70, 144);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 20);
            this.label5.TabIndex = 4;
            this.label5.Text = "Maximum Jerk";
            // 
            // m_ok
            // 
            this.m_ok.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ok.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.m_ok.Location = new System.Drawing.Point(241, 180);
            this.m_ok.Name = "m_ok";
            this.m_ok.Size = new System.Drawing.Size(75, 32);
            this.m_ok.TabIndex = 5;
            this.m_ok.Text = "OK";
            this.m_ok.UseVisualStyleBackColor = true;
            // 
            // m_cancel
            // 
            this.m_cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.m_cancel.Location = new System.Drawing.Point(322, 180);
            this.m_cancel.Name = "m_cancel";
            this.m_cancel.Size = new System.Drawing.Size(75, 32);
            this.m_cancel.TabIndex = 6;
            this.m_cancel.Text = "Cancel";
            this.m_cancel.UseVisualStyleBackColor = true;
            // 
            // m_width_text
            // 
            this.m_width_text.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_width_text.Location = new System.Drawing.Point(188, 13);
            this.m_width_text.Name = "m_width_text";
            this.m_width_text.Size = new System.Drawing.Size(209, 26);
            this.m_width_text.TabIndex = 7;
            // 
            // m_length_text
            // 
            this.m_length_text.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_length_text.Location = new System.Drawing.Point(188, 45);
            this.m_length_text.Name = "m_length_text";
            this.m_length_text.Size = new System.Drawing.Size(209, 26);
            this.m_length_text.TabIndex = 8;
            // 
            // m_velocity_text
            // 
            this.m_velocity_text.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_velocity_text.Location = new System.Drawing.Point(188, 77);
            this.m_velocity_text.Name = "m_velocity_text";
            this.m_velocity_text.Size = new System.Drawing.Size(209, 26);
            this.m_velocity_text.TabIndex = 9;
            // 
            // m_accel_text
            // 
            this.m_accel_text.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_accel_text.Location = new System.Drawing.Point(188, 109);
            this.m_accel_text.Name = "m_accel_text";
            this.m_accel_text.Size = new System.Drawing.Size(209, 26);
            this.m_accel_text.TabIndex = 10;
            // 
            // m_jerk_text
            // 
            this.m_jerk_text.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_jerk_text.Location = new System.Drawing.Point(188, 141);
            this.m_jerk_text.Name = "m_jerk_text";
            this.m_jerk_text.Size = new System.Drawing.Size(209, 26);
            this.m_jerk_text.TabIndex = 11;
            // 
            // RobotParametersForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(419, 221);
            this.Controls.Add(this.m_jerk_text);
            this.Controls.Add(this.m_accel_text);
            this.Controls.Add(this.m_velocity_text);
            this.Controls.Add(this.m_length_text);
            this.Controls.Add(this.m_width_text);
            this.Controls.Add(this.m_cancel);
            this.Controls.Add(this.m_ok);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "RobotParametersForm";
            this.Text = "Robot Parameters";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button m_ok;
        private System.Windows.Forms.Button m_cancel;
        private System.Windows.Forms.TextBox m_width_text;
        private System.Windows.Forms.TextBox m_length_text;
        private System.Windows.Forms.TextBox m_velocity_text;
        private System.Windows.Forms.TextBox m_accel_text;
        private System.Windows.Forms.TextBox m_jerk_text;
    }
}