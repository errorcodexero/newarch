using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    public abstract class DriveModifier
    {
        public abstract Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path, PathSegment[] segs);
    }
}
