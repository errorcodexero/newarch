using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class DistanceVelocityConstraint : TimingContraint
    {
        [JsonProperty(PropertyName = "type")]
        public string ConstraintType;

        [JsonProperty(PropertyName = "distance")]
        public double Distance;

        [JsonProperty(PropertyName = "velocity")]
        public double Velocity;

        public DistanceVelocityConstraint()
        {
            ConstraintType = "distance_velocity";
            Distance = 0.0;
            Velocity = 0.0;
        }

        public DistanceVelocityConstraint(double dist ,double vel)
        {
            ConstraintType = "distance_velocity";
            Distance = dist;
            Velocity = vel;
        }
    }
}
