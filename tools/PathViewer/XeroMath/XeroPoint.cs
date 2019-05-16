using System;

namespace XeroMath
{
    public class XeroPoint
    {
        public double X;
        public double Y;

        public XeroPoint()
        {
            X = 0.0;
            Y = 0.0 ;
        }

        public XeroPoint(double x, double y)
        {
            X = x;
            Y = y;
        }

        public static XeroPoint FromAngleMagnitude(double angle, double magnitude)
        {
            double x = magnitude * Math.Cos(angle);
            double y = magnitude * Math.Sin(angle);

            return new XeroPoint(x, y);
        }
    }
}
