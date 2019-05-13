using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonConverter(typeof(TimingConstraintJsonConverter))]
    public abstract class TimingContraint
    {
        public TimingContraint()
        {
        }

        public abstract TimingContraint Clone();
    }
}
