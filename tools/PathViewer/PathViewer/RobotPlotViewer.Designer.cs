namespace PathViewer
{
    partial class RobotPlotViewer
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.m_chart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.m_chart)).BeginInit();
            this.SuspendLayout();
            // 
            // m_path_chart
            // 
            chartArea1.Name = "ChartArea1";
            this.m_chart.ChartAreas.Add(chartArea1);
            this.m_chart.Dock = System.Windows.Forms.DockStyle.Fill;
            legend1.Name = "Legend1";
            this.m_chart.Legends.Add(legend1);
            this.m_chart.Location = new System.Drawing.Point(0, 0);
            this.m_chart.Name = "m_path_chart";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.m_chart.Series.Add(series1);
            this.m_chart.Size = new System.Drawing.Size(687, 499);
            this.m_chart.TabIndex = 0;
            this.m_chart.Text = "Path";
            // 
            // RobotPlotViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_chart);
            this.Name = "RobotPlotViewer";
            this.Size = new System.Drawing.Size(687, 499);
            ((System.ComponentModel.ISupportInitialize)(this.m_chart)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart m_chart;
    }
}
