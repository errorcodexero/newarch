using System;
using System.Collections.Generic;
using PathViewer;

namespace CheesyPoofsGenerator
{
    public class CheesyPoofsGen : PathGenerator
    {
        public CheesyPoofsGen()
        {
        }

        public override string Name
        {
            get { return "CheesyPoofs Path Generation"; }
        }
        public override Version Version
        {
            get { return new Version(1, 0); }
        }

        public override void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines)
        {
            for(int i = 0; i < path.Points.Length - 1; i++)
            {
                Spline x, y;
                GenerateSpline(path.Points[i], path.Points[i + 1], out x, out y);

                xsplines.Add(x);
                ysplines.Add(y);
            }
        }

        public override PathSegment[] GenerateDetailedPath(RobotPath path)
        {
            return null;
        }

        private void GenerateSpline(WayPoint p0, WayPoint p1, out Spline xsp, out Spline  ysp)
        {
            double scale = 1.2 * p0.Distance(p1);
            double x0 = p0.X;
            double x1 = p1.X;
            double dx0 = Math.Cos(p0.Heading) * scale;
            double dx1 = Math.Cos(p1.Heading) * scale;
            double ddx0 = 0.0;
            double ddx1 = 0.0;

            double y0 = p0.Y;
            double y1 = p1.Y;
            double dy0 = Math.Sin(p0.Heading) * scale;
            double dy1 = Math.Sin(p1.Heading) * scale;
            double ddy0 = 0.0;
            double ddy1 = 0.0;

            double ax = -6 * x0 - 3 * dx0 - 0.5 * ddx0 + 0.5 * ddx1 - 3 * dx1 + 6 * x1;
            double bx = 15 * x0 + 8 * dx0 + 1.5 * ddx0 - ddx1 + 7 * dx1 - 15 * x1;
            double cx = -10 * x0 - 6 * dx0 - 1.5 * ddx0 + 0.5 * ddx1 - 4 * dx1 + 10 * x1;
            double dx = 0.5 * ddx0;
            double ex = dx0;
            double fx = x0;

            double ay = -6 * y0 - 3 * dy0 - 0.5 * ddy0 + 0.5 * ddy1 - 3 * dy1 + 6 * y1;
            double by = 15 * y0 + 8 * dy0 + 1.5 * ddy0 - ddy1 + 7 * dy1 - 15 * y1;
            double cy = -10 * y0 - 6 * dy0 - 1.5 * ddy0 + 0.5 * ddy1 - 4 * dy1 + 10 * y1;
            double dy = 0.5 * ddy0;
            double ey = dy0;
            double fy = y0;

            xsp = new Spline(new double[] { fx, ex, dx, cx, bx, ax });
            ysp = new Spline(new double[] { fy, ey, dy, cy, by, ay });
        }
    }
}
