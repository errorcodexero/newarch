using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    public abstract class PathGenerator
    {
        public abstract string Name
        {
            get;
        }

        public abstract Version Version
        {
            get;
        }

        public abstract void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines);

        public abstract PathSegment[] GenerateDetailedPath(RobotPath path);
    }
}
