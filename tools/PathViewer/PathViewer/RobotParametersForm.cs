using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PathViewer
{
    public partial class RobotParametersForm : Form
    {
        public double RobotWidth;
        public double RobotLength;
        public double MaxVelocity;
        public double MaxAccel;
        public double MaxJerk;

        public RobotParametersForm()
        {
            InitializeComponent();

        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            m_width_text.Text = RobotWidth.ToString("F2");
            m_length_text.Text = RobotLength.ToString("F2");
            m_velocity_text.Text = MaxVelocity.ToString("F2");
            m_accel_text.Text = MaxAccel.ToString("F2");
            m_jerk_text.Text = MaxAccel.ToString("F2");
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            bool cancel = false;
            base.OnClosing(e);

            if (!cancel && !Double.TryParse(m_width_text.Text, out RobotWidth))
            {
                MessageBox.Show("Invalid value for robot width, expected floating point number", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                cancel = true;
            }
            if (!cancel && !Double.TryParse(m_length_text.Text, out RobotLength))
            {
                MessageBox.Show("Invalid value for robot length, expected floating point number", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                cancel = true;
            }
            if (!cancel && !Double.TryParse(m_velocity_text.Text, out MaxVelocity))
            {
                MessageBox.Show("Invalid value for robot maximum velocity, expected floating point number", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                cancel = true;
            }
            if (!cancel && !Double.TryParse(m_accel_text.Text, out MaxAccel))
            {
                MessageBox.Show("Invalid value for robot maximum acceleration, expected floating point number", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                cancel = true;
            }
            if (!cancel && !Double.TryParse(m_jerk_text.Text, out MaxJerk))
            {
                MessageBox.Show("Invalid value for maximum jerk, expected floating point number", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                cancel = true;
            }

            e.Cancel = cancel;
        }
    }
}
