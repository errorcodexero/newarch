using System;
using System.Collections.Generic;

namespace PathViewer
{
    class UndoState
    {
        private int m_serial;
        private PathFile m_file;
        private Tuple<string, string> m_selected_path = null;
        private int m_selected_index;
        private string m_selected_group = null;

        public UndoState(int serial, PathFile pf)
        {
            m_file = new PathFile(pf);
            m_serial = serial;
            m_selected_index = -1;
        }

        public PathFile File
        {
            get { return m_file; }
        }

        public bool HasSelectedWaypoint
        {
            get { return m_selected_index != -1; }
        }

        public int SelectedIndex
        {
            get { return m_selected_index; }
            set { m_selected_index = value; }
        }

        public bool HasSelectedPath
        {
            get { return m_selected_path != null; }
        }

        public Tuple<string, string> SelectedPath
        {
            get { return m_selected_path; }
        }

        public void SetSelectedPath(PathGroup group, RobotPath path)
        {
            m_selected_path = new Tuple<string, string>(group.Name, path.Name);
        }

        public bool HasSelectedGroup
        {
            get { return m_selected_group != null; }
        }

        public string SelectedGroup
        {
            get { return m_selected_group; }
            set { m_selected_group = value; }
        }
    }
}
