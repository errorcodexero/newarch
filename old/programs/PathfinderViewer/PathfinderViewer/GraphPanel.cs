using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PathfinderViewer
{
    public partial class GraphPanel : UserControl
    {
        #region private member variables
        SplitContainer m_vertical_top;
        SplitContainer m_vertical_bottom;
        SplitContainer m_horizontal;
        IList<UserControl> m_graphs;
        #endregion

        #region public constructors
        public GraphPanel()
        {
            m_graphs = new List<UserControl>();
            InitializeComponent();
            UpdateGraphs();
        }
        #endregion

        #region public methods
        public void AddGraph(UserControl c)
        {
            if (m_graphs.Count == 4)
                return;

            m_graphs.Add(c);
            UpdateGraphs();
        }

        public void RemoveGraph(UserControl c)
        {
            if (m_graphs.Contains(c))
            {
                m_graphs.Remove(c);
                UpdateGraphs();
            }
        }

        public void SetTimeWindow(double mintime, double maxtime)
        {
            foreach(UserControl c in m_graphs)
            {
                if (c is PathViewControl)
                {
                    PathViewControl vc = c as PathViewControl;
                    vc.SetTimeWindow(mintime, maxtime);
                }
                else if (c is PositionVelocityViewControl)
                {
                    PositionVelocityViewControl vc = c as PositionVelocityViewControl;
                    vc.SetTimeWindow(mintime, maxtime);
                }
            }
        }
        #endregion

        #region private methods
        private void DisconnectGraphs()
        {
            foreach(UserControl c in m_graphs)
            {
                if (c.Parent != null)
                {
                    c.Parent.Controls.Remove(c);
                    c.Parent = null;
                }
            }
        }

        private void DestroyOneSplitter(Control c)
        {
            if (c.Parent != null && !c.Parent.IsDisposed)
                c.Parent.Controls.Remove(c);

            c.Parent = null;
            c.Dispose();
        }

        private void DestroySplitters()
        {
            if (m_vertical_bottom != null)
                DestroyOneSplitter(m_vertical_bottom);

            if (m_vertical_top != null)
                DestroyOneSplitter(m_vertical_top);

            if (m_horizontal != null)
                DestroyOneSplitter(m_horizontal);

            m_vertical_top = null;
            m_vertical_bottom = null;
            m_horizontal = null;
        }

        private void CreateSplitters()
        {
            switch(m_graphs.Count)
            {
                case 0:
                    //
                    // Now graphs, do nothing
                    //
                    break;

                case 1:
                    //
                    // One graph, we need no splitter as we will dock the
                    // graph in the GraphPanel directly.
                    //
                    Controls.Add(m_graphs[0]);
                    break;

                case 2:
                    //
                    // Two graphs, split them left and right, by creating a single
                    // splitter container
                    //
                    m_vertical_top = new SplitContainer();
                    m_vertical_top.Orientation = Orientation.Vertical;
                    m_vertical_top.Dock = DockStyle.Fill;
                    Controls.Add(m_vertical_top);
                    m_vertical_top.SplitterDistance = m_vertical_top.Width / 2;
                    m_vertical_top.Panel1.Controls.Add(m_graphs[0]);
                    m_vertical_top.Panel2.Controls.Add(m_graphs[1]);
                    break;

                case 3:
                    //
                    // Three graphs, two on the top, one on the bottom
                    //
                    m_horizontal = new SplitContainer();
                    m_horizontal.Orientation = Orientation.Horizontal;
                    m_horizontal.Dock = DockStyle.Fill;
                    Controls.Add(m_horizontal);
                    m_horizontal.SplitterDistance = m_horizontal.Height / 2;

                    m_vertical_top = new SplitContainer();
                    m_vertical_top.Orientation = Orientation.Vertical;
                    m_vertical_top.Dock = DockStyle.Fill;
                    m_horizontal.Panel1.Controls.Add(m_vertical_top);
                    m_vertical_top.SplitterDistance = m_vertical_top.Width / 2;

                    m_vertical_top.Panel1.Controls.Add(m_graphs[0]);
                    m_vertical_top.Panel2.Controls.Add(m_graphs[1]);
                    m_horizontal.Panel2.Controls.Add(m_graphs[2]);
                    break;

                case 4:                    
                    //
                    // Four graphs, two on the top, two on the bottom
                    //
                    m_horizontal = new SplitContainer();
                    m_horizontal.Orientation = Orientation.Horizontal;
                    m_horizontal.Dock = DockStyle.Fill;
                    Controls.Add(m_horizontal);
                    m_horizontal.SplitterDistance = m_horizontal.Height / 2;

                    m_vertical_top = new SplitContainer();
                    m_vertical_top.Orientation = Orientation.Vertical;
                    m_vertical_top.Dock = DockStyle.Fill;
                    m_horizontal.Panel1.Controls.Add(m_vertical_top);
                    m_vertical_top.SplitterDistance = m_vertical_top.Width / 2;

                    m_vertical_bottom = new SplitContainer();
                    m_vertical_bottom.Orientation = Orientation.Vertical;
                    m_vertical_bottom.Dock = DockStyle.Fill;
                    m_horizontal.Panel2.Controls.Add(m_vertical_bottom);
                    m_vertical_bottom.SplitterDistance = m_vertical_bottom.Width / 2;

                    m_vertical_top.Panel1.Controls.Add(m_graphs[0]);
                    m_vertical_top.Panel2.Controls.Add(m_graphs[1]);
                    m_vertical_bottom.Panel1.Controls.Add(m_graphs[2]);
                    m_vertical_bottom.Panel2.Controls.Add(m_graphs[3]);
                    break;
            }

            foreach (UserControl c in m_graphs)
                c.Dock = DockStyle.Fill;
        }

        private void UpdateGraphs()
        {
            DisconnectGraphs();
            DestroySplitters();
            CreateSplitters();
        }
        #endregion
    }
}
