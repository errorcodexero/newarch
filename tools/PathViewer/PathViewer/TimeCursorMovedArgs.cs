using System;

namespace PathViewer
{
    public class TimeCursorMovedArgs : EventArgs
    {
        public readonly double Time;

        public TimeCursorMovedArgs(double t)
        {
            Time = t;
        }
    }
}
