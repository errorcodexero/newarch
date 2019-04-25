using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace PathfinderViewer
{
    partial class PathViewControl : UserControl
    {
        public class Path
        {
            #region private variables
            private PathViewControl m_parent;

            private string m_name;
            private DataFile m_file;
            private DataFile.DataFileElement m_time;
            private DataFile.DataFileElement m_x;
            private DataFile.DataFileElement m_y;
            private DataFile.DataFileElement m_theta;

            private int m_start_sample;
            private int m_end_sample;

            #endregion

            public Path(PathViewControl parent, PathDescriptor desc)
            {
                m_parent = parent;
                m_name = desc.Name;
                m_file = parent.Manager[desc.File];
                m_time = m_file[desc.Time];
                m_x = m_file[desc.XVarName];
                m_y = m_file[desc.YVarName];
                m_theta = m_file[desc.ThetaName];

                m_start_sample = 0;
                m_end_sample = m_time.Values.Length;
            }

            public string Name
            {
                get { return m_name; }
            }

            public DataFile.DataFileElement X
            {
                get { return m_x; }
            }

            public DataFile.DataFileElement Y
            {
                get { return m_y; }
            }

            public DataFile.DataFileElement Theta
            {
                get { return m_theta; }
            }

            public int StartSample
            {
                get { return m_start_sample; }
            }

            public int EndSample
            {
                get { return m_end_sample; }
            }

            public void SetTimeWindow(double mintime, double maxtime)
            {
                m_start_sample = m_time.GetIndex(mintime);
                m_end_sample = m_time.GetIndex(maxtime);
                if (m_end_sample != m_time.Values.Length - 1)
                    m_end_sample++;
            }
        }

        #region private variables
        private List<Path> m_paths;
        private DataManager m_mgr ;
        #endregion

        #region public constructors
        public PathViewControl(DataManager mgr)
        {
            m_mgr = mgr;

            m_paths = new List<Path>();
            InitializeComponent();

            ContextMenu menu = new ContextMenu();
            menu.MenuItems.Add(new MenuItem("Add Path", new EventHandler(AddPathHandler)));
            menu.MenuItems.Add(new MenuItem("Delete Path", new EventHandler(DeletePathHandler)));

            ContextMenu = menu;
        }
        #endregion

        #region public properties
        public DataManager Manager
        {
            get { return m_mgr; }
        }
        #endregion

        #region public methods
        public void Clear()
        {
            m_paths.Clear();
        }

        public void AddPath(Path p)
        {
            m_paths.Add(p);
            UpdateChart() ;
        }
        #endregion

        #region public methods
        public void SetTimeWindow(double mintime, double maxtime)
        {
            foreach (Path p in m_paths)
                p.SetTimeWindow(mintime, maxtime);

            UpdateChart();
        }
        #endregion

        #region private methods
        private void AddPathHandler(object sender, EventArgs args)
        {
            SelectPathVariablesDialog dialog = new SelectPathVariablesDialog(m_mgr);
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                PathDescriptor desc = new PathDescriptor(dialog.File, dialog.NameValue, dialog.TimeVariable, dialog.XVariable, dialog.YVariable, dialog.ThetaVariable);
                Path p = new Path(this, desc);
                AddPath(p);
                Invalidate();
            }
        }

        private void DeletePathHandler(object sender, EventArgs args)
        {
        }

        private void UpdateChart()
        {
            m_chart.Series.Clear();

            foreach(Path p in m_paths)
            {
                Series s = new Series(p.Name);
                s.ChartType = SeriesChartType.Point;
                for (int i = p.StartSample; i < p.EndSample; i++)
                    s.Points.AddXY(p.X.Values[i], p.Y.Values[i]);

                m_chart.Series.Add(s);
            }

            m_chart.ChartAreas[0].AxisX.Minimum = 0;
            m_chart.ChartAreas[0].AxisX.Maximum = 648;
            m_chart.ChartAreas[0].AxisY.Minimum = 0;
            m_chart.ChartAreas[0].AxisY.Maximum = 336;

            Invalidate();
        }
        #endregion
    }
}
