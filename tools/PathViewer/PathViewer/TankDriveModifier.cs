using System;
using System.Collections.Generic;
using XeroMath;

namespace PathViewer
{
    public class TankDriveModifier : DriveModifier
    {
        public override Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path, PathSegment[] segs, double rotvel)
        {
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            if (segs == null)
                return null;

            double lvel, lacc, ljerk, lpos = 0.0 ;
            double rvel, racc, rjerk, rpos = 0.0 ;
            double plx = 0.0, ply = 0.0;
            double prx = 0.0, pry = 0.0;
            double plvel = 0.0, prvel = 0.0;
            double placc = 0.0, pracc = 0.0;

            PathSegment[] lsegs = new PathSegment[segs.Length];
            PathSegment[] rsegs = new PathSegment[segs.Length];

            result["left"] = lsegs;
            result["right"] = rsegs;

            for (int i = 0; i < segs.Length; i++)
            {
                double heading = XeroUtils.DegreesToRadians(segs[i].GetValue("heading") / 180.0 * Math.PI);
                double ca = Math.Cos(heading);
                double sa = Math.Sin(heading);

                double lx = segs[i].GetValue("x") - robot.Width * sa / 2.0;
                double ly = segs[i].GetValue("y") + robot.Width * ca / 2.0;
                double rx = segs[i].GetValue("x") + robot.Width * sa / 2.0;
                double ry = segs[i].GetValue("y") - robot.Width * ca / 2.0;

                if (i == 0)
                {
                    lvel = 0.0;
                    lacc = 0.0;
                    lpos = 0.0;
                    ljerk = 0.0;

                    rvel = 0.0;
                    racc = 0.0;
                    rpos = 0.0;
                    rjerk = 0.0;
                }
                else
                {
                    double dt = segs[i].GetValue("time") - segs[i - i].GetValue("time");
                    double ldist = Math.Sqrt((lx - plx) * (lx - plx) + (ly - ply) * (ly - ply));
                    double rdist = Math.Sqrt((rx - prx) * (rx - prx) + (ry - pry) * (ry - pry));

                    lvel = ldist / dt;
                    rvel = rdist / dt;

                    lacc = (lvel - plvel) / dt;
                    racc = (rvel - prvel) / dt;

                    ljerk = (lacc - placc) / dt;
                    rjerk = (racc - pracc) / dt;

                    lpos += ldist;
                    rpos += rdist;
                }

                PathSegment left = new PathSegment();
                left.SetValue("time", segs[i].GetValue("time"));
                left.SetValue("x", lx);
                left.SetValue("y", ly);
                left.SetValue("heading", segs[i].GetValue("heading"));
                left.SetValue("position", lpos);
                left.SetValue("velocity", lvel);
                left.SetValue("acceleration", lacc);
                left.SetValue("jerk", ljerk);
                lsegs[i] = left;

                PathSegment right = new PathSegment();
                right.SetValue("time", segs[i].GetValue("time"));
                right.SetValue("x", rx);
                right.SetValue("y", ry);
                right.SetValue("heading", segs[i].GetValue("heading"));
                right.SetValue("position", rpos);
                right.SetValue("velocity", rvel);
                right.SetValue("acceleration", racc);
                right.SetValue("jerk", rjerk);
                rsegs[i] = right;

                plx = lx;
                ply = ly;
                prx = rx;
                pry = ry;
                plvel = lvel;
                prvel = rvel;
                placc = lacc;
                pracc = racc;
            }

            return result;
        }
    }
}
