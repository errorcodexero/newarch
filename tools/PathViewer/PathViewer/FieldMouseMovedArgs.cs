using System;
using System.Drawing;

namespace PathViewer
{
    public class FieldMouseMovedArgs : EventArgs
    {
        public readonly PointF Point;

        public FieldMouseMovedArgs(PointF pt)
        {
            Point = pt;
        }
    }
}
