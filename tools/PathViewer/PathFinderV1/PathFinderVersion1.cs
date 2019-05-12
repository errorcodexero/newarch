using System;
using System.Collections.Generic;
using PathViewer;

namespace PathFinderV1
{
    public class PathFinderVersion1 : PathGenerator
    {
        public PathFinderVersion1()
        {
        }

        public override string Name
        {
            get { return "PathFinder Version One"; }
        }

        public override Version Version
        {
            get { return new Version(1, 0); }
        }


        public override void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines)
        {
        }

        public override PathSegment[] GenerateDetailedPath(RobotParams robot, RobotPath path)
        {
            return null;
        }
    }
}
