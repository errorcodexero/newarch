namespace PathfinderViewer
{
    partial class SelectPathVariablesDialog
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
            this.m_ok = new System.Windows.Forms.Button();
            this.m_cancel = new System.Windows.Forms.Button();
            this.m_data_files = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.m_name = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.m_time_variable = new System.Windows.Forms.ListBox();
            this.m_x_variable = new System.Windows.Forms.ListBox();
            this.m_y_variable = new System.Windows.Forms.ListBox();
            this.m_theta_variable = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(150, 87);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 20);
            this.label1.TabIndex = 1;
            this.label1.Text = "X Variable";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(284, 87);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(82, 20);
            this.label2.TabIndex = 2;
            this.label2.Text = "Y Variable";
            // 
            // m_ok
            // 
            this.m_ok.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.m_ok.Location = new System.Drawing.Point(459, 284);
            this.m_ok.Name = "m_ok";
            this.m_ok.Size = new System.Drawing.Size(75, 29);
            this.m_ok.TabIndex = 4;
            this.m_ok.Text = "OK";
            this.m_ok.UseVisualStyleBackColor = true;
            this.m_ok.Click += new System.EventHandler(this.OkClick);
            // 
            // m_cancel
            // 
            this.m_cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.m_cancel.Location = new System.Drawing.Point(378, 284);
            this.m_cancel.Name = "m_cancel";
            this.m_cancel.Size = new System.Drawing.Size(75, 29);
            this.m_cancel.TabIndex = 5;
            this.m_cancel.Text = "Cancel";
            this.m_cancel.UseVisualStyleBackColor = true;
            // 
            // m_data_files
            // 
            this.m_data_files.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_data_files.FormattingEnabled = true;
            this.m_data_files.Location = new System.Drawing.Point(88, 13);
            this.m_data_files.Name = "m_data_files";
            this.m_data_files.Size = new System.Drawing.Size(445, 28);
            this.m_data_files.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(415, 87);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 20);
            this.label3.TabIndex = 7;
            this.label3.Text = "Heading";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 87);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(43, 20);
            this.label4.TabIndex = 10;
            this.label4.Text = "Time";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(32, 51);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(51, 20);
            this.label5.TabIndex = 11;
            this.label5.Text = "Name";
            // 
            // m_name
            // 
            this.m_name.Location = new System.Drawing.Point(88, 48);
            this.m_name.Name = "m_name";
            this.m_name.Size = new System.Drawing.Size(445, 26);
            this.m_name.TabIndex = 12;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(49, 17);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(34, 20);
            this.label6.TabIndex = 13;
            this.label6.Text = "File";
            // 
            // m_time_variable
            // 
            this.m_time_variable.FormattingEnabled = true;
            this.m_time_variable.ItemHeight = 20;
            this.m_time_variable.Location = new System.Drawing.Point(19, 110);
            this.m_time_variable.Name = "m_time_variable";
            this.m_time_variable.Size = new System.Drawing.Size(120, 164);
            this.m_time_variable.TabIndex = 14;
            // 
            // m_x_variable
            // 
            this.m_x_variable.FormattingEnabled = true;
            this.m_x_variable.ItemHeight = 20;
            this.m_x_variable.Location = new System.Drawing.Point(154, 110);
            this.m_x_variable.Name = "m_x_variable";
            this.m_x_variable.Size = new System.Drawing.Size(120, 164);
            this.m_x_variable.TabIndex = 15;
            // 
            // m_y_variable
            // 
            this.m_y_variable.FormattingEnabled = true;
            this.m_y_variable.ItemHeight = 20;
            this.m_y_variable.Location = new System.Drawing.Point(288, 110);
            this.m_y_variable.Name = "m_y_variable";
            this.m_y_variable.Size = new System.Drawing.Size(120, 164);
            this.m_y_variable.TabIndex = 16;
            // 
            // m_theta_variable
            // 
            this.m_theta_variable.FormattingEnabled = true;
            this.m_theta_variable.ItemHeight = 20;
            this.m_theta_variable.Location = new System.Drawing.Point(414, 110);
            this.m_theta_variable.Name = "m_theta_variable";
            this.m_theta_variable.Size = new System.Drawing.Size(120, 164);
            this.m_theta_variable.TabIndex = 17;
            // 
            // SelectPathVariablesDialog
            // 
            this.AcceptButton = this.m_ok;
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.CancelButton = this.m_cancel;
            this.ClientSize = new System.Drawing.Size(545, 322);
            this.ControlBox = false;
            this.Controls.Add(this.m_theta_variable);
            this.Controls.Add(this.m_y_variable);
            this.Controls.Add(this.m_x_variable);
            this.Controls.Add(this.m_time_variable);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.m_name);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.m_data_files);
            this.Controls.Add(this.m_cancel);
            this.Controls.Add(this.m_ok);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SelectPathVariablesDialog";
            this.Text = "Select Variables For Path";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button m_ok;
        private System.Windows.Forms.Button m_cancel;
        private System.Windows.Forms.ComboBox m_data_files;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox m_name;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ListBox m_time_variable;
        private System.Windows.Forms.ListBox m_x_variable;
        private System.Windows.Forms.ListBox m_y_variable;
        private System.Windows.Forms.ListBox m_theta_variable;
    }
}