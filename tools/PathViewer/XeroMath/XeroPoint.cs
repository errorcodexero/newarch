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

        public XeroPoint Rotate(double radians)
        {
            double nx = X * Math.Cos(radians) - Y * Math.Sin(radians);
            double ny = Y * Math.Cos(radians) + X * Math.Sin(radians);
            return new XeroPoint(nx, ny);
        }

        public static XeroPoint operator+(XeroPoint p1, XeroPoint p2)
        {
            return new XeroPoint(p1.X + p2.X, p1.Y + p2.Y);
        }
    }
}
