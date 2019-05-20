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

        private enum Wheel
        {
            FL,
            FR,
            BL, 
            BR
        };

        public override Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            Dictionary<string, PathSegment[]> ret = null;

            if (Math.Abs(path.StartFacingAngle - path.EndFacingAngle) < 0.1)
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

        /// <summary>
        /// Return the velocity of the wheel on the ground to do the rotation, based on the angular velocity required
        /// </summary>
        /// <param name="robot">robot parameters, notably width and length</param>
        /// <param name="rv">the angular velocity in degrees per second</param>
        /// <returns></returns>
        private double GetGroundRotationalVelocity(RobotParams robot, double rv)
        {
            double diameter = Math.Sqrt(robot.Width * robot.Width + robot.Length * robot.Length);
            double circum = diameter * Math.PI;
            return rv / 360.0 * circum;
        }

        private XeroVector GetWheelVelocityVector(RobotParams robot, Wheel w, double velocity)
        {
            double dx = 0.0, dy = 0.0;

            switch (w)
            {
                case Wheel.FL:
                    dx = robot.Width / 2.0;
                    dy = robot.Length / 2.0;
                    break;

                case Wheel.FR:
                    dx = robot.Width / 2.0;
                    dy = -robot.Length / 2.0;
                    break;

                case Wheel.BL:
                    dx = -robot.Width / 2.0;
                    dy = robot.Length / 2.0;
                    break;

                case Wheel.BR:
                    dx = -robot.Width / 2.0;
                    dy = -robot.Length / 2.0;
                    break;
            }

            return new XeroVector(dx, dy).Normalize().Perpendicular().Scale(velocity);
        }

        private Dictionary<string, PathSegment[]> ModifyPathWithRotation(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            if (segs == null)
                return null;

            double total = segs[segs.Length - 1].GetValue("time");
            TrapezoidalProfile profile = CreateRotationProfile(robot, path.StartFacingAngle, path.EndFacingAngle);
            if (profile.TotalTime > total)
            {
                throw new Exception("this case not handled yet, rotation cannot completed in path time");
            }

            PathSegment[] fl = new PathSegment[segs.Length];
            PathSegment[] fr = new PathSegment[segs.Length];
            PathSegment[] bl = new PathSegment[segs.Length];
            PathSegment[] br = new PathSegment[segs.Length];

            result["fl"] = fl;
            result["fr"] = fr;
            result["bl"] = bl;
            result["br"] = br;

            WheelState pflst, pfrst, pblst, pbrst;
            WheelState flst = null, frst = null, blst = null, brst = null;

            double current = path.StartFacingAngle;
            for (int i = 0; i < segs.Length; i++)
            {
                double time = segs[i].GetValue("time");

                //
                // Get the required  velocity to perform the rotation.  This is the
                // linear ground based velocity for the wheel that must be applied to
                // each wheel in direction perpendicular to the vector from the center of
                // the robot to the center of the wheel.
                //
                double rv = GetGroundRotationalVelocity(robot, profile.GetVelocity(time));

                //
                // Get the velocity vector relative to each of the wheels to rotate the
                // robot based on the desired rotational velocity.
                //
                XeroVector rotflvel = GetWheelVelocityVector(robot, Wheel.FL, rv);
                XeroVector rotfrvel = GetWheelVelocityVector(robot, Wheel.FR, rv);
                XeroVector rotblvel = GetWheelVelocityVector(robot, Wheel.BL, rv);
                XeroVector rotbrvel = GetWheelVelocityVector(robot, Wheel.BR, rv);

                //
                // Get the translational velocity vector to follow the path
                //
                XeroVector trans = XeroVector.FromDegreesMagnitude(segs[i].GetValue("heading"), segs[i].GetValue("velocity"));

                //
                // For each wheel, the velocity vector is the sum of the rotational vector and the translational vector
                //
                XeroVector flv = rotflvel + trans;
                XeroVector frv = rotfrvel + trans;
                XeroVector blv = rotblvel + trans;
                XeroVector brv = rotbrvel + trans;


                if (i == 0)
                {
                }
                else
                {
                }

                pflst = flst;
                pfrst = frst;
                pblst = blst;
                pbrst = brst;
            }

            return result;
        }

        private Dictionary<string, PathSegment[]> ModifyPathSimple(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            PathSegment seg;
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
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

            XeroPose pt;

            for (int i = 0; i < segs.Length; i++)
            {
                XeroVector loc = new XeroVector(segs[i].GetValue("x"), segs[i].GetValue("y")) ;

                pt = new XeroPose(robot.Width / 2.0, robot.Length / 2.0, 0.0);
                pt = pt.RotateDegrees(path.StartFacingAngle).Translate(loc);
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                fl[i] = seg;

                pt = new XeroPose(robot.Width / 2.0, -robot.Length / 2.0);
                pt = pt.RotateDegrees(path.StartFacingAngle).Translate(loc);
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                fr[i] = seg;

                pt = new XeroPose(-robot.Width / 2.0, robot.Length / 2.0);
                pt = pt.RotateDegrees(path.StartFacingAngle).Translate(loc);
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                bl[i] = seg;

                pt = new XeroPose(-robot.Width / 2.0, -robot.Length / 2.0);
                pt = pt.RotateDegrees(path.StartFacingAngle).Translate(loc);
                seg = new PathSegment(segs[i]);
                seg.AddValue("x", pt.X);
                seg.AddValue("y", pt.Y);
                br[i] = seg;
            }

            return result;
        }
    }
}
