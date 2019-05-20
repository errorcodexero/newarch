using System;
using System.Collections.Generic;
using System.Text;

namespace XeroMath
{
    public class XeroUtils
    {
        static public double BoundRadians(double r)
        {
            if (r > Math.PI)
                r -= 2 * Math.PI;
            else if (r <= -Math.PI)
                r += 2 * Math.PI;

            return r;
        }

        static public double BoundDegrees(double d)
        {
            if (d > 180.0)
                d -= 360;
            else if (d <= -180)
                d += 360;

            return d;
        }

        static public double DegreesToRadians(double d)
        {
            return d / 180.0 * Math.PI;
        }

        static public double RadiansToDegrees(double r)
        {
            return r / Math.PI * 180.0;
        }
    }
}
