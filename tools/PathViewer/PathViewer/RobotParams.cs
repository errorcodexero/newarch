using System;
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

        [JsonProperty(PropertyName = "units")]
        public string Units;

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
            Units = "inches";
        }

        public RobotParams(string dtype, string units, double timestep, double w, double l, double v, double a, double j)
        {
            Width = w;
            Length = l;
            MaxVelocity = v;
            MaxAcceleration = a;
            MaxJerk = j;
            TimeStep = timestep;
            Units = units;

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
            Units = p.Units;
            TimeStep = p.TimeStep;
        }

        public void ConvertUnits(string oldunits, string newunits)
        {
            Length = UnitConverter.Convert(Length, oldunits, newunits);
            Width = UnitConverter.Convert(Width, oldunits, newunits);
            MaxVelocity = UnitConverter.Convert(MaxVelocity, oldunits, newunits);
            MaxAcceleration = UnitConverter.Convert(MaxAcceleration, oldunits, newunits);
            MaxJerk = UnitConverter.Convert(MaxJerk, oldunits, newunits);
        }

        static public bool IsValidDriveType(string dtype)
        {
            return Array.IndexOf(DriveTypes, dtype) != -1;
        }
    }
}
