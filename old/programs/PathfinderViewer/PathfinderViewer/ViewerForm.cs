using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PathfinderViewer
{
    partial class ViewerForm : Form
    {
        DataManager m_mgr;

        public ViewerForm()
        {
            m_mgr = new DataManager();
            InitializeComponent();

            m_slider.ValueChanged += TimeRangeChanged;
        }

        private void TimeRangeChanged(object sender, EventArgs e)
        {
            m_graphs.SetTimeWindow(m_slider.LeftValue, m_slider.RightValue);
        }

        public DataManager Manager
        {
            get { return m_mgr; }
        }

        private void TrackerValueChanged(object sender, EventArgs e)
        {
        }

        private void LoadFileCommand(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "csv files (*.csv)|*.csv|All files (*.*)|*.*";
            dialog.FilterIndex = 0;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    double mintime, maxtime;

                    m_mgr.LoadFile(dialog.FileName);
                    m_mgr.GetBounds("t", out mintime, out maxtime);
                    m_slider.Minimum = mintime;
                    m_slider.Maximum = maxtime;
                    m_slider.LeftValue = mintime;
                    m_slider.RightValue = maxtime;
                }
                catch(Exception ex)
                {
                    string msg = string.Format("Cannot load file '{0}' - {1}", dialog.FileName, ex.Message);
                    MessageBox.Show(msg, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void ExitCommand(object sender, EventArgs e)
        {
            Close();
        }

        private void CreateXYGraph(object sender, EventArgs e)
        {
            PathViewControl c = new PathViewControl(m_mgr);
            m_graphs.AddGraph(c);
        }

        private void CreatePosVelGraph(object sender, EventArgs e)
        {
            PositionVelocityViewControl c = new PositionVelocityViewControl(m_mgr);
            m_graphs.AddGraph(c);
        }
    }
}
