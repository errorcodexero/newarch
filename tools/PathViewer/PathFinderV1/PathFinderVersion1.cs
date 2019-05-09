using System;
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

        public override PathSegment[] QuickGenerate(RobotPath path)
        {
            return null;
        }

        public override PathSegment[] AccurateGenerate(RobotPath path)
        {
            return null;
        }
    }
}
