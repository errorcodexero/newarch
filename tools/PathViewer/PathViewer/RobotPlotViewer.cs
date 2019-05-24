using System;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Diagnostics;
using System.Threading;

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
        private RectangleAnnotation m_rect;
        private string m_units;
        #endregion

        #region public events
        public event EventHandler<TimeCursorMovedArgs> TimeCursorMoved;
        #endregion

        #region private delegates
        private delegate void RegenDelegate();
        private delegate void CreateAnnotationsDelegate();
        #endregion

        public RobotPlotViewer()
        {
            Debug.WriteLine("RobotPlotViewer created in thread " + Thread.CurrentThread.Name);
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

        public double Time
        {
            get { return m_time; }
            set
            {
                m_time = value;
                if (InvokeRequired)
                    Invoke(new CreateAnnotationsDelegate(CreateAnnotations));
                else
                    CreateAnnotations();
            }
        }

        private void CreateAnnotations()
        {
            if (m_line == null)
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

            if (m_rect == null)
            {
                m_rect = new RectangleAnnotation();
                m_rect.ForeColor = Color.White;
                m_rect.BackColor = Color.Blue;
                m_rect.Width = 10;
                m_rect.Height = 4;
                m_rect.Width = 2.5;
                m_rect.AnchorY = 0.0;
                m_rect.AnchorX = 0.0;
                m_rect.AxisY = m_chart.ChartAreas[0].AxisY;
                m_rect.AxisX = m_chart.ChartAreas[0].AxisX;
                m_rect.ClipToChartArea = null;
                m_chart.Annotations.Add(m_rect);
            }

            m_rect.Text = m_time.ToString("F2");
            m_rect.AnchorX = m_time;
            m_line.AnchorX = m_time;
        }

        private void M_chart_AnnotationPositionChanging(object sender, AnnotationPositionChangingEventArgs e)
        {
            if (e.Annotation == m_line)
            {
                m_rect.AnchorX = e.NewLocationX;
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
                    PathSegmentsUpdated(null, null);
                    Time = 0.0;
                }
                else
                {
                    m_chart.Series.Clear();
                }
            }
        }

        private void PathSegmentsUpdated(object sender, EventArgs e)
        {
            if (!Created)
                return;

            if (InvokeRequired)
                Invoke(new RegenDelegate(RegenerateGraph));
            else
            {
                if (Thread.CurrentThread.Name != "MainThread")
                    Debug.WriteLine("Expected InvokeRequired, got false, thread = " + Thread.CurrentThread.Name);
                RegenerateGraph();
            }
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
            s.ToolTip = "#VALX, #VALY";
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
