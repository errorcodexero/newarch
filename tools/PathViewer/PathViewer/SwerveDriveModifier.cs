using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    class SwerveDriveModifier : DriveModifier
    {
        private class WheelState
        {
            public double x ;
            public double y ;
            public double velocity ;
            public double acceleration ;
            public double heading ;

            public WheelState()
            {
                x = 0.0;
                y = 0.0;
                velocity = 0.0;
                acceleration = 0.0;
                heading = 0.0;
            }
        };

        public override Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path)
        {
            Dictionary<string, PathSegment[]> ret = null;

            if (Math.Abs(path.StartAngle - path.EndAngle) < 0.1)
                ret = ModifyPathSimple(robot, path);
            else
                ret = ModifyPathWithRotation(robot, path);

            return ret;
        }

        private TrapezoidalProfile CreateRotationProfile(RobotParams robot, double start, double end)
        {
            double diff = end - start;
            if (diff > 180.0)
                diff -= 360.0;
            else if (diff <= -180.0)
                diff += 360.0;

            TrapezoidalProfile tp = new TrapezoidalProfile(180.0, 360.0);
            tp.Update(diff, 0.0, 0.0);
            return tp;
        }

        private Dictionary<string, PathSegment[]> ModifyPathWithRotation(RobotParams robot, RobotPath path)
        {
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            PathSegment[] segs = path.Segments;
            if (segs == null)
                return null;

            TrapezoidalProfile profile = CreateRotationProfile(robot, path.StartAngle, path.EndAngle);
            if (profile.TotalTime > path.TotalTime)
                throw new Exception("this case not handled yet, rotation cannot completed in path time");

            PathSegment[] fl = new PathSegment[segs.Length];
            PathSegment[] fr = new PathSegment[segs.Length];
            PathSegment[] bl = new PathSegment[segs.Length];
            PathSegment[] br = new PathSegment[segs.Length];

            result["fl"] = fl;
            result["fr"] = fr;
            result["bl"] = bl;
            result["br"] = br;

            WheelState pfl = new WheelState();
            WheelState pfr = new WheelState();
            WheelState pbl = new WheelState();
            WheelState pbr = new WheelState();

            for (int i = 0; i < segs.Length; i++)
            {
                WheelState flst = new WheelState();
                WheelState frst = new WheelState();
                WheelState blst = new WheelState();
                WheelState brst = new WheelState();

                double time = segs[i].GetValue("time");
                double x = segs[i].GetValue("x");
                double y = segs[i].GetValue("y");
                double heading = segs[i].GetValue("heading");

                //
                // Get the required rotational velocity.  For each wheel this should be
                // orthoginal to the 
                //
                double rv = profile.GetVelocity(time);

                //
                // For each wheel there are two velocity vectors acting on the wheel.  One vector is the
                // path velocity and heading to drive from start to finish.  The second velocity
                // vector is the one rotating the wheel.
                //
                if (i == 0)
                {
                    double accel = segs[i].GetValue("acceleration");

                    flst.x = x + robot.Width / 2.0;
                    flst.y = y + robot.Length / 2.0;
                    flst.velocity = 0.0;
                    flst.heading = heading;
                    flst.acceleration = accel;

                    frst.x = x + robot.Width / 2.0;
                    frst.y = y - robot.Length / 2.0;
                    frst.heading = heading;
                    frst.velocity = 0.0;
                    frst.acceleration = accel;

                    blst.x = x - robot.Width / 2.0;
                    blst.y = y + robot.Length / 2.0;
                    blst.velocity = 0.0;
                    blst.heading = heading;
                    blst.acceleration = accel;

                    brst.x = x - robot.Width / 2.0;
                    brst.y = y - robot.Length / 2.0;
                    brst.velocity = 0.0;
                    brst.heading = heading;
                    brst.acceleration = accel;
                }
                else
                {
                    //
                    // Calculate the next state of the wheels
                    //
                }

                //
                // Store the state of each wheel
                //

                //
                // Remember the previous state
                //
                pfl = flst;
                pfr = frst;
                pbl = blst;
                pbr = brst;
            }

            return result;
        }

        private Dictionary<string, PathSegment[]> ModifyPathSimple(RobotParams robot, RobotPath path)
        {
            PathSegment seg;
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            PathSegment[] segs = path.Segments;
            if (segs == null)
                return null;

            PathSegment[] fl = new PathSegment[segs.Length];
            PathSegment[] fr = new PathSegment[segs.Length];
            PathSegment[] bl = new PathSegment[segs.Length];
            PathSegment[] br = new PathSegment[segs.Length];

            result["fl"] = fl;
            result["fr"] = fr;
            result["bl"] = bl;
            result["br"] = br;

            for (int i = 0; i < segs.Length; i++)
            {
                double x = segs[i].GetValue("x");
                double y = segs[i].GetValue("y");

                seg = new PathSegment(segs[i]);
                seg.AddValue("x", x + robot.Width / 2.0);
                seg.AddValue("y", y + robot.Length / 2.0);
                fl[i] = seg;

                seg = new PathSegment(segs[i]);
                seg.AddValue("x", x + robot.Width / 2.0);
                seg.AddValue("y", y - robot.Length / 2.0);
                fr[i] = seg;

                seg = new PathSegment(segs[i]);
                seg.AddValue("x", x - robot.Width / 2.0);
                seg.AddValue("y", y + robot.Length / 2.0);
                bl[i] = seg;

                seg = new PathSegment(segs[i]);
                seg.AddValue("x", x - robot.Width / 2.0);
                seg.AddValue("y", y - robot.Length / 2.0);
                br[i] = seg;
            }

            return result;
        }
    }
}
