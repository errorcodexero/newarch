using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
