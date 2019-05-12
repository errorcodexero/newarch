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
        public readonly ReasonType Reason;

        public enum ReasonType
        {
            Selected,
            StartChange,
            Changing,
            EndChange
        }

        public WaypointEventArgs(ReasonType t, PathGroup g, RobotPath s, WayPoint pt)
        {
            Reason = t;
            Group = g;
            Path = s;
            Point = pt;
        }
    }
}
