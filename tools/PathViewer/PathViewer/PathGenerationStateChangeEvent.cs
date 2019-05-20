using System;

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
