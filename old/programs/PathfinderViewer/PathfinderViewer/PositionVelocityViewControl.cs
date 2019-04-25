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
    partial class PositionVelocityViewControl : UserControl
    {
        private DataManager m_mgr;

        public PositionVelocityViewControl(DataManager mgr)
        {
            m_mgr = mgr;
            InitializeComponent();
        }

        public void SetTimeWindow(double mintime, double maxtime)
        {
        }
    }
}
