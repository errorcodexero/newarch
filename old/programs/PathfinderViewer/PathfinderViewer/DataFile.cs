using System;
using System.IO;
using System.Collections.Generic;

namespace PathfinderViewer
{
    class DataFile
    {
        #region public classes
        public class DataFileElement
        {
            #region private members
            private string m_name;
            private double[] m_values;
            private double m_max;
            private double m_min;
            #endregion

            public DataFileElement(string name)
            {
                m_name = name;
                m_values = new double[0];
                m_max = Double.MinValue;
                m_min = Double.MaxValue;
            }

            public string Name
            {
                get { return m_name; }
            }

            public double[] Values
            {
                get { return m_values; }
            }

            public double Maximum
            {
                get { return m_max; }
            }

            public double Minimum
            {
                get { return m_min; }
            }

            public void addValue(double value)
            {
                int newsize = m_values.Length + 1;
                Array.Resize<double>(ref m_values, newsize);
                m_values[newsize - 1] = value;

                if (value > m_max)
                    m_max = value;

                if (value < m_min)
                    m_min = value;
            }

            public int GetIndex(double value)
            {
                int left = 0;
                int right = m_values.Length - 1;

                if (value < m_values[left])
                    return left;

                if (value > m_values[right])
                    return right;

                while (true)
                {
                    if (right - left == 1)
                        break;

                    int mid = (left + right) / 2;
                    if (value < m_values[mid])
                        right = mid;
                    else
                        left = mid;
                }

                return left;
            }
        }
        #endregion

        #region private members
        private string m_filename;
        private IList<DataFileElement> m_element_array;
        #endregion

        #region public constructors
        public DataFile(string name)
        {
            m_filename = name;
            m_element_array = new List<DataFileElement>();
        }
        #endregion

        #region public properties
        public DataFileElement this[string name]
        {
            get
            {
                foreach (DataFileElement elem in m_element_array)
                {
                    if (elem.Name == name)
                        return elem;
                }

                return null;
            }
        }

        public int Count
        {
            get { return m_element_array.Count; }
        }

        public IList<string> Names
        {
            get
            {
                List<string> names = new List<string>();
                foreach(DataFileElement elem in m_element_array)
                {
                    names.Add(elem.Name);
                }

                return names;
            }
        }

        #endregion

        #region public methods
        public bool Contains(string name)
        {
            foreach(DataFileElement elem in m_element_array)
            {
                if (elem.Name == name)
                    return true;
            }

            return false;
        }

        public void Clear()
        {
            m_element_array.Clear();
        }

        public void ReadFile(string filename)
        {
            string line;
            StreamReader rdr = new StreamReader(filename);
            int which = 0;
            string[] fields;

            while ((line = rdr.ReadLine()) != null)
            {
                fields = line.Split(',');

                if (which == 0)
                {
                    createFields(fields);
                }
                else
                {
                    putData(fields);
                }

                which++;
            }

            rdr.Close();
        }
        #endregion

        #region private members
        private void createFields(string [] names)
        {
            foreach(string name in names)
            {
                DataFileElement elem = new DataFileElement(name);
                m_element_array.Add(elem);
            }
        }

        private void putData(string [] data)
        {
            if (data.Length != m_element_array.Count)
            {
                m_element_array.Clear();
                throw new Exception("Invalid data line in data file - wrong number of elements");
            }

            for(int i = 0; i < m_element_array.Count; i++)
            {
                double value;
                
                if (data[i].Length == 0)
                {
                    value = 0;
                }
                else if (data[i] == "inf")
                {
                    value = Double.PositiveInfinity;
                }
                else if (!Double.TryParse(data[i], out value))
                {
                    m_element_array.Clear();
                    throw new Exception("Invalid data line in data file - invalid value");
                }

                m_element_array[i].addValue(value);
            }
        }
        #endregion
    }
}
