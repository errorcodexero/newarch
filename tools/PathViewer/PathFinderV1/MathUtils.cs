using System;
using System.Collections.Generic;
using System.Text;

namespace PathFinderV1
{
    class MathUtils
    {
        static public double BoundRadians(double angle)
        {
            double newangle = System.Math.IEEERemainder(angle, Math.PI * 2.0);
            if (newangle < 0)
                newangle += Math.PI * 2.0;

            return newangle;
        }
    }
}
