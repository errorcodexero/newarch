using System;

namespace XeroMath
{
    public class XeroVector
    {
        #region public members
        public double X;
        public double Y;
        #endregion

        #region public constructors
        public XeroVector()
        {
            X = 0.0;
            Y = 0.0 ;
        }

        public XeroVector(double x, double y)
        {
            X = x;
            Y = y;
        }
        #endregion

        #region public properties
        public double Length
        {
            get
            {
                return Math.Sqrt(X * X + Y * Y);
            }
        }
        #endregion

        #region public methods

        public double Magnitude()
        {
            return Math.Sqrt(X * X + Y * Y);
        }

        public double AngleRadians()
        {
            return Math.Atan2(Y, X);
        }

        public double AngleDegrees()
        {
            return XeroUtils.RadiansToDegrees(AngleRadians());
        }

        public XeroVector Translate(double x, double y )
        {
            return new XeroVector(X + x, Y + y);
        }

        public XeroVector RotateRadians(double radians)
        {
            double nx = X * Math.Cos(radians) - Y * Math.Sin(radians);
            double ny = Y * Math.Cos(radians) + X * Math.Sin(radians);
            return new XeroVector(nx, ny);
        }

        public XeroVector RotateDegrees(double degrees)
        {
            return RotateRadians(XeroUtils.DegreesToRadians(degrees));
        }

        public XeroVector Perpendicular()
        {
            return new XeroVector(Y, -X);
        }

        public XeroVector Interpolate(XeroVector other, double percent)
        {
            if (percent < 0.0)
                percent = 0.0;
            else if (percent > 1.0)
                percent = 1.0;

            double x = X + (other.X - X) * percent;
            double y = Y + (other.Y - Y) * percent;
            return new XeroVector(x, y);
        }

        public XeroVector Normalize()
        {
            double r = Math.Sqrt(X * X + Y * Y);
            return new XeroVector(X / r, Y / r);
        }

        public XeroVector Scale(double s)
        {
            return new XeroVector(X * s, Y * s);
        }
        #endregion

        #region public static methods

        public static XeroVector FromRadiansMagnitude(double angle, double magnitude)
        {
            double x = magnitude * Math.Cos(angle);
            double y = magnitude * Math.Sin(angle);

            return new XeroVector(x, y);
        }

        public static XeroVector FromDegreesMagnitude(double angle, double magnitude)
        {
            return FromRadiansMagnitude(XeroUtils.DegreesToRadians(angle), magnitude);
        }

        public static double Distance(XeroVector p1, XeroVector p2)
        {
            return (p1 - p2).Length;
        }
        #endregion

        #region public operators
        public static XeroVector operator+(XeroVector p1, XeroVector p2)
        {
            return new XeroVector(p1.X + p2.X, p1.Y + p2.Y);
        }

        public static XeroVector operator-(XeroVector p1, XeroVector p2)
        {
            return new XeroVector(p1.X - p2.X, p1.Y - p2.Y);
        }

        public static XeroVector operator*(XeroVector p1, double scale)
        {
            return new XeroVector(p1.X * scale, p1.Y * scale);
        }
        #endregion
    }
}
