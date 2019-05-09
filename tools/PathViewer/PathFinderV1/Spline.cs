using System;
using System.Collections.Generic;
using System.Text;

namespace PathFinderV1
{
    class Spline
    {
        public double A;
        public double B;
        public double C;
        public double D;
        public double E;

        public double XOffset;
        public double YOffset;

        public double KnotDistance;
        public double AngleOffset;
        public double ArcLength;

        public Spline(double a, double b, double c, double d, double e)
        {
            A = a;
            B = b;
            C = c;
            D = d;
            E = e;
        }

        public Coord InterpolatePosition(double percent)
        {
            double pcnt = Math.Max(Math.Min(percent, 1.0), 0.0);
            double x = pcnt * KnotDistance;
            double y = (A * x + B) * (x * x * x * x) + (C * x + D) * x * x + E * x;

            double cos_theta = Math.Cos(AngleOffset);
            double sin_theta = Math.Sin(AngleOffset);

            return new Coord(x * cos_theta - y * sin_theta + XOffset, x * sin_theta + y * cos_theta + YOffset);
        }

        public double Derivative(double percent)
        {
            double x = percent * KnotDistance;
            return (5.0 * A * x + 4 * B) * x * x * x + (3.0 * C * x + 2.0 * D) * x + E;
        }

        public double InterpolateAngle(double percent)
        {
            double angle = Math.Atan(Derivative(percent)) + AngleOffset;
            return MathUtils.BoundRadians(angle);
        }

        public double Distance(int count)
        {
            double sample_count_d = (double)count;

            double a = A;
            double b = B;
            double c = C;
            double d = D;
            double e = E;
            double knot = KnotDistance;

            double arc_length = 0;
            double t = 0;
            double dydt = 0;

            double deriv0 = Derivative(0.0);

            double integrand = 0;
            double last_integrand = Math.Sqrt(1 + deriv0 * deriv0) / sample_count_d;

            for (int i = 0; i <= count; i++)
            {
                t = i / sample_count_d;
                dydt = Derivative(t);
                integrand = Math.Sqrt(1 + dydt * dydt) / sample_count_d;
                arc_length += (integrand + last_integrand) / 2;
                last_integrand = integrand;
            }
            
            ArcLength = knot * arc_length;
            return ArcLength;
        }

        double ProgressForDistance(double distance, int count)
        {
            double sample_count_d = (double)count;

            double a = A;
            double b = B;
            double c = C;
            double d = D;
            double e = E;
            double knot = KnotDistance;

            double arc_length = 0, t = 0, dydt = 0, last_arc_length = 0;

            double deriv0 = Derivative(0);

            double integrand = 0;
            double last_integrand = Math.Sqrt(1 + deriv0 * deriv0) / sample_count_d;

            distance /= knot;

            int i;
            for (i = 0; i <= count; i = i + 1)
            {
                t = i / sample_count_d;
                dydt = Derivative(t);
                integrand = Math.Sqrt(1 + dydt * dydt) / sample_count_d;
                arc_length += (integrand + last_integrand) / 2;
                if (arc_length > distance) break;
                last_integrand = integrand;
                last_arc_length = arc_length;
            }

            double interpolated = t;
            if (arc_length != last_arc_length)
            {
                interpolated += ((distance - last_arc_length)
                    / (arc_length - last_arc_length) - 1) / sample_count_d;
            }
            return interpolated;
        }
    }
}
