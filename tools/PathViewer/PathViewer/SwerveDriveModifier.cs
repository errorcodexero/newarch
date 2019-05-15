using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    class SwerveDriveModifier : DriveModifier
    {
        public override Dictionary<string, PathSegment[]> ModifyPath(RobotParams robot, RobotPath path)
        {
            Dictionary<string, PathSegment[]> ret = null;

            if (Math.Abs(path.StartAngle - path.EndAngle) < 0.1)
                ret = ModifyPathSimple(robot, path);
            else
                ret = ModifyPathWithRotation(robot, path);

            return ret;
        }

        private Dictionary<string, PathSegment[]> ModifyPathWithRotation(RobotParams robot, RobotPath path)
        {
            return null;
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
                seg.AddValue("x", x - robot.Width / 2.0);
                seg.AddValue("y", y + robot.Length / 2.0);
                fl[i] = seg;

                seg = new PathSegment(segs[i]);
                seg.AddValue("x", x + robot.Width / 2.0);
                seg.AddValue("y", y + robot.Length / 2.0);
                fr[i] = seg;

                seg = new PathSegment(segs[i]);
                seg.AddValue("x", x - robot.Width / 2.0);
                seg.AddValue("y", y - robot.Length / 2.0);
                bl[i] = seg;

                seg = new PathSegment(segs[i]);
                seg.AddValue("x", x + robot.Width / 2.0);
                seg.AddValue("y", y - robot.Length / 2.0);
                br[i] = seg;
            }

            return result;
        }
    }
}
