﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace PathViewer
{
    public partial class RobotPlotViewer : UserControl
    {

        #region private members
        private RobotParams m_robot;
        private PathGenerator m_generator;
        private RobotPath m_path;
        #endregion

        #region private delegates
        private delegate void RegenDelegate();
        #endregion

        public RobotPlotViewer()
        {
            InitializeComponent();
            DoubleBuffered = true;
        }

        public RobotParams Robot
        {
            get { return m_robot; }
            set { m_robot = value; }
        }

        public PathGenerator Generator
        {
            get { return m_generator; }
            set { m_generator = value; }
        }

        public RobotPath Path
        {
            get { return m_path; }
            set
            {
                if (m_path != null)
                    m_path.SegmentsUpdated -= PathSegmentsUpdated;

                m_path = value;
                m_path.SegmentsUpdated += PathSegmentsUpdated;
                RegenerateGraph();
            }
        }

        private void PathSegmentsUpdated(object sender, EventArgs e)
        {
            if (InvokeRequired)
                Invoke(new RegenDelegate(RegenerateGraph));
            else
                RegenerateGraph();
        }

        public void RegenerateGraph()
        {
            if (m_path != null)
            {
                PathSegment[] segs = m_path.Segments;
                if (segs != null)
                {
                    GenerateDataSeries(segs);
                    Invalidate();
                }
            }
        }

        private void CreateSeries(string name, AxisType a, PathSegment[] segs)
        {
            Series s = new Series(name);
            s.YAxisType = a ;
            s.ChartType = SeriesChartType.Line;
            for(int i = 0; i < segs.Length; i++)
            {
                double t = segs[i].GetValue("time");
                double v = segs[i].GetValue(name);
                s.Points.AddXY(t, v);
                s.BorderWidth = 5;
            }

            m_chart.Series.Add(s);
        }

        private void GenerateDataSeries(PathSegment[] segs)
        {
            ChartArea area = m_chart.ChartAreas[0];

            m_chart.Series.Clear();
            CreateSeries("position", AxisType.Primary, segs);
            CreateSeries("velocity", AxisType.Secondary, segs);
            CreateSeries("acceleration", AxisType.Secondary, segs);

            area.AxisX.Title = "time (sec)";
            area.AxisY.Title = "distance (inches)";
            area.AxisY2.Title = "velocity/acceleration";
        }
    }
}
