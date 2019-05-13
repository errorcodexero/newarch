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

        [JsonProperty(PropertyName = "after")]
        public double After;

        [JsonProperty(PropertyName = "before")]
        public double Before;

        [JsonProperty(PropertyName = "velocity")]
        public double Velocity;

        public DistanceVelocityConstraint()
        {
            ConstraintType = "distance_velocity";
            Before = 0.0;
            After = 0.0;
            Velocity = 0.0;
        }

        public DistanceVelocityConstraint(double after, double before ,double vel)
        {
            ConstraintType = "distance_velocity";
            After = after;
            Before = before;
            Velocity = vel;
        }

        public override TimingContraint Clone()
        {
            return new DistanceVelocityConstraint(After, Before, Velocity);
        }
    }
}
