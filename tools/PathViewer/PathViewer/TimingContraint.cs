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
