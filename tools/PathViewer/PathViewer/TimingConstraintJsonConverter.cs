using System;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace PathViewer
{
    class TimingConstraintJsonConverter : JsonConverter<TimingContraint>
    {
        public override bool CanWrite => false;

        public override void WriteJson(JsonWriter writer, TimingContraint value, JsonSerializer serializer)
        {
            throw new NotSupportedException("Should only be used during deserializing");
        }

        public override TimingContraint ReadJson(JsonReader reader, Type objectType, TimingContraint existingValue, bool hasExistingValue, JsonSerializer serializer)
        {
            TimingContraint ret = null;

            if (reader.TokenType == JsonToken.Null)
                return ret;

            JObject obj = JObject.Load(reader);
            JToken token = obj["type"];
            if (token == null)
                throw new JsonSerializationException("Error deserializing TimingContstraint, no 'type' field");

            if (token.ToString() == "centripetal")
            {
                double maxacc = 0.0;

                try
                {
                    token = obj["maxaccel"];
                    if (token == null)
                        throw new JsonSerializationException("Error deserializing TimingContstraint (type = DistanceVelocityConstraint), no 'maxaccel' field");

                    maxacc = token.ToObject<Double>();
                }
                catch (Exception ex)
                {
                    throw new JsonSerializationException("Error deserializing TimingContstraint, 'maxaccel' field was not a valid number", ex);
                }

                ret = new CentripetalAccelerationConstraint(maxacc);
            }
            else if (token.ToString() == "distance_velocity")
            {
                double before =0.0, after = 0.0, velocity = 0.0;

                try
                {
                    token = obj["after"];
                    if (token == null)
                        throw new JsonSerializationException("Error deserializing TimingContstraint (type = DistanceVelocityConstraint), no 'after' field");

                    after = token.ToObject<Double>();
                }
                catch(Exception ex)
                {
                    throw new JsonSerializationException("Error deserializing TimingContstraint, 'after' field was not a valid number", ex);
                }

                try
                {
                    token = obj["before"];
                    if (token == null)
                        throw new JsonSerializationException("Error deserializing TimingContstraint (type = DistanceVelocityConstraint), no 'before' field");

                    before = token.ToObject<Double>();
                }
                catch (Exception ex)
                {
                    throw new JsonSerializationException("Error deserializing TimingContstraint, 'before' field was not a valid number", ex);
                }

                try
                {
                    token = obj["velocity"];
                    if (token == null)
                        throw new JsonSerializationException("Error deserializing TimingContstraint (type = DistanceVelocityConstraint), no 'velocity' field");

                    velocity = token.ToObject<Double>();
                }
                catch (Exception ex)
                {
                    throw new JsonSerializationException("Error deserializing TimingContstraint, 'velocity' field was not a valid number", ex);
                }

                ret = new DistanceVelocityConstraint(after, before, velocity);
            }
            else
            {
                throw new JsonSerializationException("Error deserializing TimingContstraint, type '" + token.ToString() + "' is not valid");
            }

            return ret;
        }
    }
}
