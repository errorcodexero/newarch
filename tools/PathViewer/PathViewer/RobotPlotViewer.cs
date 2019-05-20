using System;
using System.Drawing;
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
        private double m_time;
        private LineAnnotation m_line;
        private string m_units;
        #endregion

        #region public events
        public event EventHandler<TimeCursorMovedArgs> TimeCursorMoved;
        #endregion

        #region private delegates
        private delegate void RegenDelegate();
        #endregion

        public RobotPlotViewer()
        {
            InitializeComponent();
            DoubleBuffered = true;
        }

        public string Units
        {
            get { return m_units; }
            set { m_units = value; Invalidate(); }
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

        public double HighlightTime
        {
            get { return m_time; }
            set
            {
                m_time = value;
                m_chart.Annotations.Clear();
                m_line = null;

                if (m_time < Double.MaxValue)
                {
                    m_line = new VerticalLineAnnotation();
                    m_line.LineColor = Color.Black;
                    m_line.LineDashStyle = ChartDashStyle.Dash;
                    m_line.AxisX = m_chart.ChartAreas[0].AxisX;
                    m_line.IsInfinitive = true;
                    m_line.ClipToChartArea = m_chart.ChartAreas[0].Name;
                    m_line.LineWidth = 3;
                    m_line.AnchorX = m_time;
                    m_line.AllowMoving = true;
                    m_chart.Annotations.Add(m_line);

                    m_chart.AnnotationPositionChanging += M_chart_AnnotationPositionChanging;
                }
            }
        }

        private void M_chart_AnnotationPositionChanging(object sender, AnnotationPositionChangingEventArgs e)
        {
            if (e.Annotation == m_line)
            {
                EventHandler<TimeCursorMovedArgs> handler = TimeCursorMoved;
                handler?.Invoke(this, new TimeCursorMovedArgs(e.NewLocationX));
            }
        }

        public RobotPath Path
        {
            get { return m_path; }
            set
            {
                if (m_path != null)
                    m_path.SegmentsUpdated -= PathSegmentsUpdated;

                m_path = value;
                if (m_path != null)
                {
                    m_path.SegmentsUpdated += PathSegmentsUpdated;
                    RegenerateGraph();
                }
                else
                {
                    m_chart.Series.Clear();
                }
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
            m_chart.Series.Clear();
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
            area.AxisY.Title = "distance (" + m_units + ")";
            area.AxisY2.Title = "velocity/acceleration";

            m_chart.ChartAreas[0].RecalculateAxesScale();
        }
    }
}
