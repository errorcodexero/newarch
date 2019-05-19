using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    class UnitConverter
    {
        private struct Units
        {
            public List<string> UnitNames;
            public double ToInches;

            public Units(List<string> names, double to)
            {
                UnitNames = names;
                ToInches = to;
            }
        };

        private static Units[] UnitsEntries =
        {
            new Units(new List<string>(){"inch", "in", "inches"}, 1.0),
            new Units(new List<string>(){"feet", "foot", "ft"}, 12.0),
            new Units(new List<string>(){"cm"}, 0.393701),
            new Units(new List<string>(){"m", "meter", "meters"}, 39.3701),
        };


        public static double Convert(double inval, string from, string to)
        {
            bool found = false;
            double v = inval;

            found = false;
            for (int i = 0; i < UnitsEntries.Length && !found; i++)
            {
                if (UnitsEntries[i].UnitNames.Contains(from))
                {
                    found = true;
                    v *= UnitsEntries[i].ToInches;
                }
            }

            if (!found)
                throw new Exception("UnitsConverter - from units '" + from + "'not known");

            found = false;
            for(int i = 0; i < UnitsEntries.Length && !found; i++)
            {
                if (UnitsEntries[i].UnitNames.Contains(to))
                {
                    found = true;
                    v /= UnitsEntries[i].ToInches;
                }
            }

            if (!found)
                throw new Exception("UnitsConverter - to units '" + to + "'not known");

            return v;
        }
    }
}
