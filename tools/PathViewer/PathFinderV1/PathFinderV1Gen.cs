using System;
using System.Collections.Generic;
using PathViewer;

namespace PathFinderV1
{
    public class PathFinderV1Gen : PathGenerator
    {
        #region public constructor
        public PathFinderV1Gen()
        {
        }
        #endregion

        #region public properties
        public override string Name
        {
            get { return "PathFinder Version 1"; }
        }

        public override Version Version
        {
            get { return new Version(1, 0); }
        }
        #endregion

        #region public methods
        public override void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines)
        {
            for (int i = 0; i < path.Points.Length - 1; i++)
            {
                Spline x, y;
                GenerateSpline(path.Points[i], path.Points[i + 1], out x, out y);

                xsplines.Add(x);
                ysplines.Add(y);
            }
        }

        public override PathSegment[] GenerateDetailedPath(RobotParams robot, RobotPath path)
        {
            return null;
        }
        #endregion

        #region private methods


        private void GenerateSpline(WayPoint p0, WayPoint p1, out Spline xsp, out Spline ysp)
        {
            double dist = p0.Distance(p1);
            double aoffset = Math.Atan2(p1.Y - p0.Y, p1.X - p0.X);

            double a0_delta = Math.Tan(MathUtils.BoundRadians(MathUtils.DegreesToRadians(p0.Heading) - aoffset));
            double a1_delta = Math.Tan(MathUtils.BoundRadians(MathUtils.DegreesToRadians(p1.Heading) - aoffset));

            double[] coeffs = new double[6];
            coeffs[5] = -(3 * (a0_delta + a1_delta)) / (dist * dist * dist * dist);
            coeffs[4] = (8 * a0_delta + 7 * a1_delta) / (dist * dist * dist);
            coeffs[3] = -(6 * a0_delta + 4 * a1_delta) / (dist * dist);
            coeffs[2] = 0.0;
            coeffs[1] = a0_delta;
            coeffs[0] = 0.0;

            xsp = new Spline(dist, coeffs);
            xsp.XOffset = p0.X;
            xsp.YOffset = p0.Y;
            xsp.AngleOffset = MathUtils.RadiansToDegrees(aoffset);
            ysp = null;
        }
        #endregion
    }
}
