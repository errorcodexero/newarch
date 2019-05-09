using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;


namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class PathFile
    {
        [JsonProperty(PropertyName = "groups")]
        public PathGroup[] Groups;

        private bool m_dirty;
        private string m_filename;

        public PathFile()
        {
            Groups = new PathGroup[0];
        }

        public bool IsDirty
        {
            get { return m_dirty; }
            set { m_dirty = true; }
        }

        public string PathName
        {
            get { return m_filename; }
            set { m_filename = value; }
        }

        public PathGroup FindGroupByPath(RobotPath p)
        {
            foreach (PathGroup gr in Groups)
            {
                foreach(RobotPath pa in gr.Paths)
                {
                    if (pa == p)
                        return gr;
                }
            }

            return null;
        }

        public PathGroup FindGroupByName(string name)
        {
            foreach (PathGroup gr in Groups)
            {
                if (gr.Name == name)
                    return gr;
            }

            return null;
        }

        public RobotPath FindPathByName(string group, string path)
        {
            PathGroup gr = FindGroupByName(group);
            if (gr == null)
                return null;

            return gr.FindPathByName(path);
        }

        public void AddPathGroup(string name)
        {
            PathGroup group = new PathGroup(name);
            Array.Resize<PathGroup>(ref Groups, Groups.Length + 1);
            Groups[Groups.Length - 1] = group;
        }

        public void AddPath(string group, string path)
        {
            PathGroup gr = FindGroupByName(group);
            if (gr == null)
                return;

            gr.AddPath(path);
        }

        public bool RenameGroup(string oldname, string newname)
        {
            PathGroup gr = FindGroupByName(oldname);
            if (gr == null)
                return false;

            gr.Name = newname;
            m_dirty = true;
            return true;
        }

        public bool RenamePath(string group, string oldname, string newname)
        {
            PathGroup gr = FindGroupByName(group);
            if (gr == null)
                return false;

            if (!gr.RenamePath(oldname, newname))
                return false;

            m_dirty = true;
            return true;
        }

        public bool FindPathByWaypoint(WayPoint pt, out PathGroup group, out RobotPath path)
        {
            group = null;
            path = null;

            foreach(PathGroup gr in Groups)
            {
                foreach(RobotPath pa in gr.Paths)
                {
                    foreach (WayPoint wy in pa.Points)
                    {
                        if (wy == pt)
                        {
                            group = gr;
                            path = pa;
                            return true;
                        }
                    }
                }
            }

            return false;
        }
    }
}
