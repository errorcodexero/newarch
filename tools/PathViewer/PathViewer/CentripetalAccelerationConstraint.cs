using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;


namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class CentripetalAccelerationConstraint : TimingContraint
    {
        [JsonProperty(PropertyName = "type")]
        public readonly string ConstraintType;

        [JsonProperty(PropertyName = "maxaccel")]
        public readonly double MaxAccel;

        public CentripetalAccelerationConstraint()
        {
            ConstraintType = "centripetal";
            MaxAccel = 0.0;
        }

        public CentripetalAccelerationConstraint(double accel)
        {
            ConstraintType = "centripetal";
            MaxAccel = accel;
        }

        public override TimingContraint Clone()
        {
            return new CentripetalAccelerationConstraint(MaxAccel);
        }
    }
}
