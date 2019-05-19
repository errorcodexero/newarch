using System;
using System.Collections.Generic;
using XeroMath;


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

        public override Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            Dictionary<string, PathSegment[]> ret = null;

            if (Math.Abs(path.StartAngle - path.EndAngle) < 0.1)
                ret = ModifyPathSimple(robot, path, segs);
            else
                ret = ModifyPathWithRotation(robot, path, segs);

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

        XeroPoint CalculateWheelRotationalSpeed(double wheel_angle, double robot_angle, double rv)
        {
            return new XeroPoint(0, 0);
        }

        private Dictionary<string, PathSegment[]> ModifyPathWithRotation(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            if (segs == null)
                return null;

            double total = segs[segs.Length - 1].GetValue("time");
            TrapezoidalProfile profile = CreateRotationProfile(robot, path.StartAngle, path.EndAngle);
            if (profile.TotalTime > total)
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

            double current = path.StartAngle;
            double wheel_angle_fl = Math.Atan2(robot.Width / 2.0, robot.Length / 2.0);
            double wheel_angle_fr = Math.Atan2(robot.Width / 2.0, -robot.Length / 2.0);
            double wheel_angle_bl = Math.Atan2(-robot.Width / 2.0, robot.Length / 2.0);
            double wheel_angle_br = Math.Atan2(-robot.Width / 2.0, -robot.Length / 2.0);

            for (int i = 0; i < segs.Length; i++)
            {
                WheelState flst = new WheelState();
                WheelState frst = new WheelState();
                WheelState blst = new WheelState();
                WheelState brst = new WheelState();

                WheelState pathst = new WheelState();
                double time = segs[i].GetValue("time");
                pathst.x = segs[i].GetValue("x");
                pathst.y = segs[i].GetValue("y");
                pathst.heading = segs[i].GetValue("heading") / 180.0 * Math.PI;
                pathst.velocity = segs[i].GetValue("velocity");
                pathst.acceleration = segs[i].GetValue("acceleration");

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
                    flst.x = pathst.x + robot.Width / 2.0;
                    flst.y = pathst.y + robot.Length / 2.0;
                    flst.velocity = 0.0;
                    flst.heading = pathst.heading;
                    flst.acceleration = pathst.acceleration;

                    frst.x = pathst.x + robot.Width / 2.0;
                    frst.y = pathst.y - robot.Length / 2.0;
                    frst.heading = pathst.heading;
                    frst.velocity = 0.0;
                    frst.acceleration = pathst.acceleration;

                    blst.x = pathst.x - robot.Width / 2.0;
                    blst.y = pathst.y + robot.Length / 2.0;
                    blst.velocity = 0.0;
                    blst.heading = pathst.heading;
                    blst.acceleration = pathst.acceleration;

                    brst.x = pathst.x - robot.Width / 2.0;
                    brst.y = pathst.y - robot.Length / 2.0;
                    brst.velocity = 0.0;
                    brst.heading = pathst.heading;
                    brst.acceleration = pathst.acceleration;
                }
                else
                {
                    //
                    // One vector is the velocity vector for the translational motion
                    //
                    XeroPoint translation = XeroPoint.FromAngleMagnitude(pathst.heading, pathst.velocity);

                    //
                    // The second vector is the rototational vector for the rotation.
                    //
                    XeroPoint rvector = CalculateWheelRotationalSpeed(wheel_angle_fl, current, rv);

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

        private Dictionary<string, PathSegment[]> ModifyPathSimple(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            PathSegment seg;
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            if (segs == null)
                return null;

            double angle = path.StartAngle / 180.0 * Math.PI;

            PathSegment[] fl = new PathSegment[segs.Length];
            PathSegment[] fr = new PathSegment[segs.Length];
            PathSegment[] bl = new PathSegment[segs.Length];
            PathSegment[] br = new PathSegment[segs.Length];

            result["fl"] = fl;
            result["fr"] = fr;
            result["bl"] = bl;
            result["br"] = br;

            XeroPoint pt;

            for (int i = 0; i < segs.Length; i++)
            {
                XeroPoint loc = new XeroPoint(segs[i].GetValue("x"), segs[i].GetValue("y")) ;

                pt = new XeroPoint(robot.Width / 2.0, robot.Length / 2.0);
                pt = pt.Rotate(angle) + loc;
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                fl[i] = seg;

                pt = new XeroPoint(robot.Width / 2.0, -robot.Length / 2.0);
                pt = pt.Rotate(angle) + loc;
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                fr[i] = seg;

                pt = new XeroPoint(-robot.Width / 2.0, robot.Length / 2.0);
                pt = pt.Rotate(angle) + loc;
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                bl[i] = seg;

                pt = new XeroPoint(-robot.Width / 2.0, -robot.Length / 2.0);
                pt = pt.Rotate(angle) + loc;
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                br[i] = seg;
            }

            return result;
        }
    }
}
