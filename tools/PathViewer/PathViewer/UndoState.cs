using System;
using System.Collections.Generic;

namespace PathViewer
{
    class UndoState
    {
        private int m_serial;
        private PathFile m_file;
        private List<Tuple<string, string>> m_selected_paths;
        private int m_selected_index;
        private string m_selected_group;
        private string m_selected_path;

        public UndoState(int serial, PathFile pf)
        {
            m_file = new PathFile(pf);
            m_serial = serial;
            m_selected_paths = new List<Tuple<string, string>>();
            m_selected_index = -1;
        }

        public bool HasSelectedWaypoint
        {
            get { return m_selected_index != -1; }
        }

        public PathFile File
        {
            get { return m_file; }
        }

        public List<Tuple<string, string>> SelectedPaths
        {
            get { return m_selected_paths; }
        }

        public string SelectedGroup
        {
            get { return m_selected_group; }
        }

        public string SelectedPath
        {
            get { return m_selected_path; }
        }

        public int SelectedIndex
        {
            get { return m_selected_index; }
        }

        public void AddPath(string group, string path)
        {
            Tuple<string, string> t = new Tuple<string, string>(group, path);
            m_selected_paths.Add(t);
        }

        public void AddSelectedWaypoint(string group, string path, int index)
        {
            m_selected_group = group;
            m_selected_path = path;
            m_selected_index = index;
        }
    }
}
