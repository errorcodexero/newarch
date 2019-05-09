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
        public abstract PathSegment[] QuickGenerate(RobotPath path);
        public abstract PathSegment[] AccurateGenerate(RobotPath path);
    }
}
