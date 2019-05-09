using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class LocationVelocityConstraint : TimingContraint
    {
        [JsonProperty(PropertyName = "type")]
        public string ConstraintType;

        [JsonProperty(PropertyName = "X")]
        public double X;

        [JsonProperty(PropertyName = "Y")]
        public double Y;

        [JsonProperty(PropertyName = "distance")]
        public double Distance;

        [JsonProperty(PropertyName = "velocity")]
        public double Velocity;

        LocationVelocityConstraint()
        {
            ConstraintType = "location_velocity";
        }

        LocationVelocityConstraint(double x, double y, double dist, double vel)
        {
            ConstraintType = "location_velocity";
            X = x;
            Y = y;
            Distance = dist;
            Velocity = vel;
        }
    }
}
