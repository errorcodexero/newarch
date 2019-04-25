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
    partial class SelectPathVariablesDialog : Form
    {
        private DataManager m_mgr;

        public string File;
        public string NameValue;
        public string TimeVariable;
        public string XVariable;
        public string YVariable;
        public string ThetaVariable;

        public SelectPathVariablesDialog(DataManager mgr)
        {
            m_mgr = mgr;

            InitializeComponent();

            foreach (string file in mgr.FileNames)
                m_data_files.Items.Add(file);

            m_data_files.SelectedIndexChanged += DataFileChangedHandler;

            if (m_data_files.Items.Count > 0)
                m_data_files.SelectedIndex = 0;
        }

        private void PopulateList(ListBox box, DataFile file)
        {
            box.Items.Clear();
            foreach (string name in file.Names)
                box.Items.Add(name);
        }

        private void DataFileChangedHandler(object sender, EventArgs e)
        {
            DataFile file = m_mgr[m_data_files.SelectedItem.ToString()];
            if (file != null)
            {
                PopulateList(m_time_variable, file);
                PopulateList(m_x_variable, file);
                PopulateList(m_y_variable, file);
                PopulateList(m_theta_variable, file);

                if (file.Contains("t") && file.Contains("pose_x") && file.Contains("pose_y") && file.Contains("pose_theta"))
                {
                    m_name.Text = "TargetPos";
                    m_time_variable.SelectedItem = "t";
                    m_x_variable.SelectedItem = "pose_x";
                    m_y_variable.SelectedItem = "pose_y";
                    m_theta_variable.SelectedItem = "pose_theta";
                }
                else if (file.Contains("t") && file.Contains("path_x") && file.Contains("path_y") && file.Contains("path_theta"))
                {
                    m_name.Text = "Path";
                    m_time_variable.SelectedItem = "t";
                    m_x_variable.SelectedItem = "path_x";
                    m_y_variable.SelectedItem = "path_y";
                    m_theta_variable.SelectedItem = "path_theta";
                }
                else if (file.Contains("t") && file.Contains("patx") && file.Contains("paty") && file.Contains("pata"))
                {
                    m_name.Text = "ActualPos";
                    m_time_variable.SelectedItem = "t";
                    m_x_variable.SelectedItem = "patx";
                    m_y_variable.SelectedItem = "paty";
                    m_theta_variable.SelectedItem = "pata";
                }
            }
        }

        private void OkClick(object sender, EventArgs e)
        {
            if (m_data_files.SelectedIndex == -1)
            {
                MessageBox.Show("Must select a data file", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                DialogResult = DialogResult.None;
                return;
            }

            if (m_time_variable.SelectedItems.Count == 0)
            {
                MessageBox.Show("Must select a 'time' variable", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                DialogResult = DialogResult.None;
                return;
            }

            if (m_x_variable.SelectedItems.Count == 0)
            {
                MessageBox.Show("Must select an 'x' variable", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                DialogResult = DialogResult.None;
                return;
            }

            if (m_y_variable.SelectedItems.Count == 0)
            {
                MessageBox.Show("Must select an 'y' variable", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                DialogResult = DialogResult.None;
                return;
            }

            if (m_theta_variable.SelectedItems.Count == 0)
            {
                MessageBox.Show("Must select a 'heading' variable", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                DialogResult = DialogResult.None;
                return;
            }

            File = m_data_files.SelectedItem.ToString();
            NameValue = m_name.Text;
            TimeVariable = m_time_variable.SelectedItem.ToString();
            XVariable = m_x_variable.SelectedItem.ToString();
            YVariable = m_y_variable.SelectedItem.ToString();
            ThetaVariable = m_theta_variable.SelectedItem.ToString();
        }
    }
}
