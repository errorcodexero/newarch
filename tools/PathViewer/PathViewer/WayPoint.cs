using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class WayPoint
    {
        [JsonProperty(PropertyName = "X")]
        public double X;

        [JsonProperty(PropertyName = "Y")]
        public double Y;
       
        [JsonProperty(PropertyName = "Heading")]
        public double Heading;

        [JsonProperty(PropertyName = "Velocity")]
        public double Velocity;

        public WayPoint()
        {
            X = 0.0;
            Y = 0.0;
            Heading = 0.0;
            Velocity = 0.0;
        }

        public WayPoint(double x, double y, double h, double v)
        {
            X = x;
            Y = y;
            Heading = h;
            Velocity = v;
        }
    }
}
