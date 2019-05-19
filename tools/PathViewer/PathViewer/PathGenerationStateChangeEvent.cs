using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    public class PathGenerationStateChangeEvent : EventArgs
    {
        public readonly int JobsRunning;
        public readonly int JobsQueued;

        public PathGenerationStateChangeEvent(int running, int queued)
        {
            JobsRunning = running;
            JobsQueued = queued;
        }

        public int TotalJobs
        {
            get { return JobsRunning + JobsQueued; }
        }
    }
}
