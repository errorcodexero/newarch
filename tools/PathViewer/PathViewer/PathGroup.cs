using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class PathGroup
    {
        [JsonProperty(PropertyName = "name")]
        public string Name;

        [JsonProperty(PropertyName = "paths")]
        public RobotPath[] Paths;

        public PathGroup()
        {
            Name = string.Empty;
            Paths = new RobotPath[0];
        }

        public PathGroup(string name)
        {
            Name = name;
            Paths = new RobotPath[0];
        }

        public RobotPath FindPathByName(string name)
        {
            foreach(RobotPath p in Paths)
            {
                if (p.Name == name)
                    return p;
            }

            return null; 
        }

        public bool RenamePath(string oldname, string newname)
        {
            RobotPath pt = FindPathByName(oldname);
            if (pt == null)
                return false;

            pt.Name = newname;
            return true;
        }

        public void AddPath(string name)
        {
            RobotPath path = new RobotPath(name);
            Array.Resize<RobotPath>(ref Paths, Paths.Length + 1);
            Paths[Paths.Length - 1] = path;

            path.AddPoint(new WayPoint(0.0, 0.0, 0.0, 0.0));
            path.AddPoint(new WayPoint(100.0, 0.0, 0.0, 0.0));
        }
    }
}
