using System;
using System.Collections.Generic;

namespace PathViewer
{
    public abstract class DriveModifier
    {
        public class VelocitySplitException : Exception
        {
        };

        public abstract Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path, PathSegment[] segs, double rotvel);
    }
}
