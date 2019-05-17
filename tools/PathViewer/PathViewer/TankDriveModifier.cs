using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    public class TankDriveModifier : DriveModifier
    {
        public override Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path)
        {
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            PathSegment[] segs = path.Segments;
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
                double heading = MathUtils.DegreesToRadians(segs[i].GetValue("heading") / 180.0 * Math.PI);
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
                left.AddValue("time", segs[i].GetValue("time"));
                left.AddValue("x", lx);
                left.AddValue("y", ly);
                left.AddValue("heading", segs[i].GetValue("heading"));
                left.AddValue("position", lpos);
                left.AddValue("velocity", lvel);
                left.AddValue("acceleration", lacc);
                left.AddValue("jerk", ljerk);
                lsegs[i] = left;

                PathSegment right = new PathSegment();
                right.AddValue("time", segs[i].GetValue("time"));
                right.AddValue("x", rx);
                right.AddValue("y", ry);
                right.AddValue("heading", segs[i].GetValue("heading"));
                right.AddValue("position", rpos);
                right.AddValue("velocity", rvel);
                right.AddValue("acceleration", racc);
                right.AddValue("jerk", rjerk);
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
