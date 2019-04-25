using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathfinderViewer
{
    class PathDescriptor
    {
        #region private member variables
        private string m_file;
        private string m_name;
        private string m_time;
        private string m_x_name;
        private string m_y_name;
        private string m_theta_name;
        #endregion

        #region public constructors
        public PathDescriptor(string file, string name, string time, string x, string y, string theta)
        {
            m_file = file;
            m_name = name;
            m_time = time;
            m_x_name = x;
            m_y_name = y;
            m_theta_name = theta;
        }
        #endregion

        #region public properties
        public string File
        {
            get { return m_file; }
        }

        public string Name
        {
            get { return m_name; }
        }

        public string Time
        {
            get { return m_time; }
        }

        public string XVarName
        {
            get { return m_x_name; }
        }

        public string YVarName
        {
            get { return m_y_name; }
        }

        public string ThetaName
        {
            get { return m_theta_name; }
        }
        #endregion
    }


}
