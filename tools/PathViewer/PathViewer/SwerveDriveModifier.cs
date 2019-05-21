using System;
using System.Collections.Generic;
using XeroMath;


namespace PathViewer
{
    class SwerveDriveModifier : DriveModifier
    {
        private enum Wheel
        {
            FL,
            FR,
            BL, 
            BR
        };

        public override Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            return ModifyPathWithRotation(robot, path, segs);
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
        private double RotationalToGround(RobotParams robot, double rv)
        {
            double diameter = Math.Sqrt(robot.Width * robot.Width + robot.Length * robot.Length);
            double circum = diameter * Math.PI;
            return rv / 360.0 * circum;
        }

        private XeroVector GetWheelPerpendicularVector(RobotParams robot, Wheel w, double magnitude)
        {
            double dx = 0.0, dy = 0.0;

            switch (w)
            {
                case Wheel.FL:
                    dx = robot.Length / 2.0;
                    dy = robot.Width / 2.0;
                    break;

                case Wheel.FR:
                    dx = robot.Length / 2.0;
                    dy = -robot.Width / 2.0;
                    break;

                case Wheel.BL:
                    dx = -robot.Length / 2.0;
                    dy = robot.Width / 2.0;
                    break;

                case Wheel.BR:
                    dx = -robot.Length / 2.0;
                    dy = -robot.Width / 2.0;
                    break;
            }

            return new XeroVector(dx, dy).Normalize().Perpendicular().Scale(magnitude);
        }

        private Dictionary<string, PathSegment[]> ModifyPathWithRotation(RobotParams robot, RobotPath path, PathSegment[] segs)
        {
            Dictionary<string, PathSegment[]> result = new Dictionary<string, PathSegment[]>();
            if (segs == null)
                return null;

            double total = segs[segs.Length - 1].GetValue("time");
            TrapezoidalProfile profile = CreateRotationProfile(robot, path.StartFacingAngle, path.EndFacingAngle);
            if (profile.TotalTime + path.FacingAngleRotationDelay > total)
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

            for (int i = 0; i < segs.Length; i++)
            {
                //
                // The time in the path
                //
                double time = segs[i].GetValue("time");


                //
                // The current facing angle based on the rotational trapezoidal profile
                //
                double current = XeroUtils.BoundDegrees(path.StartFacingAngle + profile.GetDistance(time - path.FacingAngleRotationDelay));

                //
                // Get the required velocity to perform the rotation.  This is the
                // linear ground based velocity for the wheel that must be applied to
                // each wheel in direction perpendicular to the vector from the center of
                // the robot to the center of the wheel.
                //
                double rv = RotationalToGround(robot, profile.GetVelocity(time - path.FacingAngleRotationDelay));
                double ra = RotationalToGround(robot, profile.GetAcceleration(time - path.FacingAngleRotationDelay));

                //
                // Get the velocity vector and acceleration vector relative to each of the 
                // wheels to rotate the robot based on the desired rotational velocity.
                //
                XeroVector rotflvel = GetWheelPerpendicularVector(robot, Wheel.FL, rv);
                XeroVector rotfrvel = GetWheelPerpendicularVector(robot, Wheel.FR, rv);
                XeroVector rotblvel = GetWheelPerpendicularVector(robot, Wheel.BL, rv);
                XeroVector rotbrvel = GetWheelPerpendicularVector(robot, Wheel.BR, rv);

                XeroVector rotflacc = GetWheelPerpendicularVector(robot, Wheel.FL, ra);
                XeroVector rotfracc = GetWheelPerpendicularVector(robot, Wheel.FR, ra);
                XeroVector rotblacc = GetWheelPerpendicularVector(robot, Wheel.BL, ra);
                XeroVector rotbracc = GetWheelPerpendicularVector(robot, Wheel.BR, ra);

                //
                // Get the translational velocity vector to follow the path
                //
                XeroVector pathvel = XeroVector.FromDegreesMagnitude(segs[i].GetValue("heading"), segs[i].GetValue("velocity")).RotateDegrees(-current);
                XeroVector pathacc = XeroVector.FromDegreesMagnitude(segs[i].GetValue("heading"), segs[i].GetValue("acceleration")).RotateDegrees(-current);



                //
                // For each wheel, the velocity vector is the sum of the rotational vector and the translational vector
                //
                XeroVector flv = rotflvel + pathvel;
                XeroVector frv = rotfrvel + pathvel;
                XeroVector blv = rotblvel + pathvel;
                XeroVector brv = rotbrvel + pathvel;
                XeroVector fla = rotflacc + pathvel;
                XeroVector fra = rotfracc + pathvel;
                XeroVector bla = rotblacc + pathvel;
                XeroVector bra = rotbracc + pathvel;

                //
                // Now calculate the wheel positions based on the path position and the
                // facing angle
                //
                XeroVector flpos = new XeroVector(robot.Length / 2.0, robot.Width / 2.0).RotateDegrees(current).Translate(segs[i].GetValue("x"), segs[i].GetValue("y"));
                XeroVector frpos = new XeroVector(robot.Length / 2.0, -robot.Width / 2.0).RotateDegrees(current).Translate(segs[i].GetValue("x"), segs[i].GetValue("y"));
                XeroVector blpos = new XeroVector(-robot.Length / 2.0, robot.Width / 2.0).RotateDegrees(current).Translate(segs[i].GetValue("x"), segs[i].GetValue("y"));
                XeroVector brpos = new XeroVector(-robot.Length / 2.0, -robot.Width / 2.0).RotateDegrees(current).Translate(segs[i].GetValue("x"), segs[i].GetValue("y"));

                PathSegment newseg = new PathSegment(segs[i]);
                newseg.SetValue("x", flpos.X);
                newseg.SetValue("y", flpos.Y);
                newseg.SetValue("velocity", flv.Magnitude());
                newseg.SetValue("heading", flv.AngleDegrees());
                newseg.SetValue("acceleration", fla.Magnitude());
                fl[i] = newseg;

                newseg = new PathSegment(segs[i]);
                newseg.SetValue("x", frpos.X);
                newseg.SetValue("y", frpos.Y);
                newseg.SetValue("velocity", frv.Magnitude());
                newseg.SetValue("heading", frv.AngleDegrees());
                newseg.SetValue("acceleration", fra.Magnitude());
                fr[i] = newseg;

                newseg = new PathSegment(segs[i]);
                newseg.SetValue("x", blpos.X);
                newseg.SetValue("y", blpos.Y);
                newseg.SetValue("velocity", blv.Magnitude());
                newseg.SetValue("heading", blv.AngleDegrees());
                newseg.SetValue("acceleration", bla.Magnitude());
                bl[i] = newseg;

                newseg = new PathSegment(segs[i]);
                newseg.SetValue("x", brpos.X);
                newseg.SetValue("y", brpos.Y);
                newseg.SetValue("velocity", brv.Magnitude());
                newseg.SetValue("heading", brv.AngleDegrees());
                newseg.SetValue("acceleration", bra.Magnitude());
                br[i] = newseg;
            }

            return result;
        }
    }
}
