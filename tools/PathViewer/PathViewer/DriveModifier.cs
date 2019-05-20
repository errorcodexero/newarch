using System.Collections.Generic;

namespace PathViewer
{
    public abstract class DriveModifier
    {
        public abstract Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path, PathSegment[] segs);
    }
}
