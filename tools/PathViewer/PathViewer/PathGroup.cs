using System;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class PathGroup
    {
        [JsonProperty(PropertyName = "name")]
        public string Name;

        [JsonProperty(PropertyName = "paths")]
        public RobotPath[] Paths;

        public PathGroup()
        {
            Name = string.Empty;
            Paths = new RobotPath[0];
        }

        public PathGroup(string name)
        {
            Name = name;
            Paths = new RobotPath[0];
        }

        public PathGroup(PathGroup pg)
        {
            Name = pg.Name;
            Paths = new RobotPath[pg.Paths.Length] ;

            for(int i = 0; i < Paths.Length; i++)
            {
                Paths[i] = new RobotPath(pg.Paths[i]);
            }
        }

        public RobotPath FindPathByName(string name)
        {
            foreach(RobotPath p in Paths)
            {
                if (p.Name == name)
                    return p;
            }

            return null; 
        }

        public void RemovePath(string name)
        {
            int index = -1;

            for(int i = 0; i < Paths.Length; i++)
            {
                if (Paths[i].Name == name)
                {
                    index = i;
                    break;
                }
            }

            if (index != -1)
            {
                RobotPath[] temp = new RobotPath[Paths.Length - 1];
                if (index > 0)
                    Array.Copy(Paths, 0, temp, 0, index);

                if (index < Paths.Length - 1)
                    Array.Copy(Paths, index + 1, temp, index, Paths.Length - index - 1);

                Paths = temp;
            }
        }

        public bool RenamePath(string oldname, string newname)
        {
            RobotPath pt = FindPathByName(oldname);
            if (pt == null)
                return false;

            pt.Name = newname;
            return true;
        }

        public void AddPath(RobotParams robot, string name)
        {
            RobotPath path = new RobotPath(name);
            path.MaxVelocity = robot.MaxVelocity;
            path.MaxAcceleration = robot.MaxAcceleration;
            path.MaxJerk = robot.MaxJerk;

            Array.Resize<RobotPath>(ref Paths, Paths.Length + 1);
            Paths[Paths.Length - 1] = path;

            double length = UnitConverter.Convert(100.0, "inches", robot.Units);

            path.AddPoint(new WayPoint(0.0, 0.0, 0.0, 0.0));
            path.AddPoint(new WayPoint(length, 0.0, 0.0, 0.0));
        }

        public void AddPath(RobotPath path)
        {
            Array.Resize<RobotPath>(ref Paths, Paths.Length + 1);
            Paths[Paths.Length - 1] = path;
        }
    }
}
