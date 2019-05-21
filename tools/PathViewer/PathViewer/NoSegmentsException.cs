using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    class NoSegmentsException : Exception
    {
        public readonly RobotPath Path;

        public NoSegmentsException(RobotPath path)
        {
            Path = path;
        }
    }
}
