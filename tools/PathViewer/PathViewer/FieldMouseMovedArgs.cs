using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
