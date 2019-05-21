using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace PathViewer
{
    class PathFileDiff
    {
        private class PathToCompare
        {
            public readonly PathGroup PathGroupOne;
            public readonly RobotPath PathOne ;
            public readonly PathGroup PathGroupTwo;
            public readonly RobotPath PathTwo;

            public PathToCompare(PathGroup pg1, RobotPath p1, PathGroup pg2, RobotPath p2)
            {
                PathGroupOne = pg1;
                PathOne = p1;
                PathGroupTwo = pg2;
                PathTwo = p2;
            }
        };

        static private double kEpsilon = 0.00001;

        public static void Diff(RobotParams p1, RobotParams p2, List<string> diff)
        {
            if (p1.DriveType != p2.DriveType)
                diff.Add("DriveType:p1 = " + p1.DriveType + ", p2 = " + p2.DriveType);

            if (p1.Units != p2.Units)
                diff.Add("Units: p1 = " + p1.Units + ", p2 = " + p2.Units);

            if (Math.Abs(p1.Length - p2.Length) > kEpsilon)
                diff.Add("Length: p1 = " + p1.Length.ToString() + ", p2 = " + p2.Length.ToString());

            if (Math.Abs(p1.Width - p2.Width) > kEpsilon)
                diff.Add("Width: p1 = " + p1.Width.ToString() + ", p2 = " + p2.Width.ToString());

            if (Math.Abs(p1.MaxVelocity - p2.MaxVelocity) > kEpsilon)
                diff.Add("MaxVelocity: p1 = " + p1.MaxVelocity.ToString() + ", p2 = " + p2.MaxVelocity.ToString());

            if (Math.Abs(p1.MaxAcceleration - p2.MaxAcceleration) > kEpsilon)
                diff.Add("MaxAcceleration: p1 = " + p1.MaxAcceleration.ToString() + ", p2 = " + p2.MaxAcceleration.ToString());

            if (Math.Abs(p1.MaxJerk - p2.MaxJerk) > kEpsilon)
                diff.Add("MaxJerk: p1 = " + p1.MaxJerk.ToString() + ", p2 = " + p2.MaxJerk.ToString());

            if (Math.Abs(p1.TimeStep - p2.TimeStep) > kEpsilon)
                diff.Add("TimeStep: p1 = " + p1.TimeStep.ToString() + ", p2 = " + p2.TimeStep.ToString());
        }

        public static void Diff(PathGroup[] p1, PathGroup [] p2, List<string> diff)
        {
            List<Tuple<PathGroup, PathGroup>> todiff = new List<Tuple<PathGroup, PathGroup>>();

            foreach(PathGroup gr in p1)
            {
                PathGroup second = Array.Find(p2, group => group.Name == gr.Name);
                if (second == null)
                {
                    diff.Add("p2 missing path group '" + gr.Name + "'");
                }
                else
                {
                    var todo = new Tuple<PathGroup, PathGroup>(gr, second);
                    todiff.Add(todo);
                }
            }

            foreach(PathGroup gr in p2)
            {
                PathGroup first = Array.Find(p1, group => group.Name == gr.Name);
                if (first == null)
                    diff.Add("p1 missing path group '" + gr.Name + "'");
            }

            foreach (Tuple<PathGroup, PathGroup> todo in todiff)
                Diff(todo.Item1, todo.Item2, diff);
        }

        public static void Diff(PathGroup p1, PathGroup p2, List<string> diff)
        {
            List<PathToCompare> todiff = new List<PathToCompare>();

            foreach(RobotPath path in p1.Paths)
            {
                RobotPath second = Array.Find(p2.Paths, search => search.Name == path.Name);
                if (second == null)
                {
                    diff.Add("p2 missing path in group '" + p1.Name + "', path '" + path.Name + "'");
                }
                else
                {
                    PathToCompare pcmp = new PathToCompare(p1, path, p2, second);
                    todiff.Add(pcmp);
                }
            }

            foreach(RobotPath path in p2.Paths)
            {
                RobotPath first = Array.Find(p1.Paths, search => search.Name == path.Name);
                if (first == null)
                {
                    diff.Add("p1 missing path in group '" + p2.Name + "', path '" + path.Name + "'");
                }
            }

            foreach (PathToCompare p in todiff)
                Diff(p.PathGroupOne, p.PathOne, p.PathGroupTwo, p.PathTwo, diff);
        }

        public static void Diff(PathGroup pgr1, RobotPath p1, PathGroup pgr2, RobotPath p2, List<string> diff)
        {
            if (Math.Abs(p1.MaxVelocity - p2.MaxVelocity) > kEpsilon)
                diff.Add("MaxVelocity: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.MaxVelocity.ToString() + ", p2 = " + p2.MaxVelocity.ToString());

            if (Math.Abs(p1.MaxAcceleration - p2.MaxAcceleration) > kEpsilon)
                diff.Add("MaxAcceleration: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.MaxAcceleration.ToString() + ", p2 = " + p2.MaxAcceleration.ToString());

            if (Math.Abs(p1.MaxJerk - p2.MaxJerk) > kEpsilon)
                diff.Add("MaxJerk: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.MaxJerk.ToString() + ", p2 = " + p2.MaxJerk.ToString());

            if (Math.Abs(p1.StartVelocity - p2.StartVelocity) > kEpsilon)
                diff.Add("StartVelocity: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.StartVelocity.ToString() + ", p2 = " + p2.StartVelocity.ToString());

            if (Math.Abs(p1.EndVelocity - p2.EndVelocity) > kEpsilon)
                diff.Add("EndVelocity: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.EndVelocity.ToString() + ", p2 = " + p2.EndVelocity.ToString());

            if (Math.Abs(p1.StartFacingAngle - p2.StartFacingAngle) > kEpsilon)
                diff.Add("StartFacingAngle: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.StartFacingAngle.ToString() + ", p2 = " + p2.StartFacingAngle.ToString());

            if (Math.Abs(p1.EndFacingAngle - p2.EndFacingAngle) > kEpsilon)
                diff.Add("EndFacingAngle: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.EndFacingAngle.ToString() + ", p2 = " + p2.EndFacingAngle.ToString());

            if (Math.Abs(p1.FacingAngleRotationDelay - p2.FacingAngleRotationDelay) > kEpsilon)
                diff.Add("FacingAngleRotationDelay: p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.FacingAngleRotationDelay.ToString() + ", p2 = " + p2.FacingAngleRotationDelay.ToString());

            // Constraints

            // Waypoints
            if (p1.Points.Length != p2.Points.Length)
            {
                diff.Add("Waypoints: p1 has " + p1.Points.Length.ToString() + ", p2 has " + p2.Points.Length.ToString());
            }
            else
            {
                for(int i = 0; i < p1.Points.Length; i++)
                {
                    if (Math.Abs(p1.Points[i].X - p2.Points[i].X) > kEpsilon)
                        diff.Add("X: WayPoint " + i.ToString() + "p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.Points[i].X.ToString() + ", p2 = " + p2.Points[i].X.ToString());

                    if (Math.Abs(p1.Points[i].Y - p2.Points[i].Y) > kEpsilon)
                        diff.Add("Y: WayPoint " + i.ToString() + "p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.Points[i].Y.ToString() + ", p2 = " + p2.Points[i].Y.ToString());

                    if (Math.Abs(p1.Points[i].Heading - p2.Points[i].Heading) > kEpsilon)
                        diff.Add("Heading: WayPoint " + i.ToString() + "p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.Points[i].Heading.ToString() + ", p2 = " + p2.Points[i].Heading.ToString());

                    if (Math.Abs(p1.Points[i].Velocity - p2.Points[i].Velocity) > kEpsilon)
                        diff.Add("Velocity: WayPoint " + i.ToString() + "p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.Points[i].Velocity.ToString() + ", p2 = " + p2.Points[i].Velocity.ToString());

                    if (Math.Abs(p1.Points[i].Velocity - p2.Points[i].Velocity) > kEpsilon)
                        diff.Add("Velocity: WayPoint " + i.ToString() + "p1 = " + "PathGroup '" + pgr1.Name + "', Path '" + p1.Name + "'" + p1.Points[i].Velocity.ToString() + ", p2 = " + p2.Points[i].Velocity.ToString());
                }
            }
        }


        public static void Diff(PathFile p1, PathFile p2, List<string> diff)
        {
            Diff(p1.Robot, p2.Robot, diff);
            Diff(p1.Groups, p2.Groups, diff);
        }

        public static void Diff(PathFile p1, PathFile p2)
        {
            List<string> diff = new List<string>();
            Diff(p1, p2, diff);

            foreach (string line in diff)
                Debug.WriteLine(line);
        }
    }
}
