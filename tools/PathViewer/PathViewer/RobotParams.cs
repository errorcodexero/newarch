using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class RobotParams
    {
        [JsonProperty(PropertyName = "width")]
        public double Width;

        [JsonProperty(PropertyName = "length")]
        public double Length;

        [JsonProperty(PropertyName = "maxvelocity")]
        public double MaxVelocity;

        [JsonProperty(PropertyName = "maxacceleration")]
        public double MaxAcceleration;

        [JsonProperty(PropertyName = "maxjerk")]
        public double MaxJerk;

        [JsonProperty(PropertyName ="drivetype")]
        public string DriveType;

        [JsonProperty(PropertyName = "timestep")]
        public double TimeStep;

        public static string SwerveDriveType = "swerve";
        public static string TankDriveType = "tank";

        public static string[] DriveTypes =
        {
            SwerveDriveType,
            TankDriveType
        };

        public RobotParams()
        {
            Width = 0.0;
            Length = 0.0;
            MaxAcceleration = 0.0;
            MaxVelocity = 0.0;
            MaxJerk = 0.0;
            TimeStep = 0.02;
            DriveType = TankDriveType;
        }

        public RobotParams(string dtype, double timestep, double w, double l, double v, double a, double j)
        {
            Width = w;
            Length = l;
            MaxVelocity = v;
            MaxAcceleration = a;
            MaxJerk = j;
            TimeStep = timestep;

            if (!IsValidDriveType(dtype))
                throw new Exception("invalid drive type '" + dtype + "'");

            DriveType = dtype;
        }

        public RobotParams(RobotParams p)
        {
            Width = p.Width;
            Length = p.Length;
            MaxVelocity = p.MaxVelocity;
            MaxAcceleration = p.MaxAcceleration;
            MaxJerk = p.MaxJerk;
            DriveType = p.DriveType;
            TimeStep = p.TimeStep;
        }

        static public bool IsValidDriveType(string dtype)
        {
            return Array.IndexOf(DriveTypes, dtype) != -1;
        }
    }
}
