using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    public class WaypointEventArgs
    {
        public readonly PathGroup Group;
        public readonly RobotPath Path;
        public readonly WayPoint Point;

        public WaypointEventArgs(PathGroup g, RobotPath s, WayPoint pt)
        {
            Group = g;
            Path = s;
            Point = pt;
        }
    }
}
