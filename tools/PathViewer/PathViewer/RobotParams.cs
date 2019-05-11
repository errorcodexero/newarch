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

        public RobotParams()
        {
            Width = 0.0;
            Length = 0.0;
            MaxAcceleration = 0.0;
            MaxVelocity = 0.0;
            MaxJerk = 0.0;
        }

        public RobotParams(double w, double l, double v, double a, double j)
        {
            Width = w;
            Length = l;
            MaxVelocity = v;
            MaxAcceleration = a;
            MaxJerk = j;
        }
    }
}
