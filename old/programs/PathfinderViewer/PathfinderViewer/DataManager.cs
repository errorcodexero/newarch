using System.Collections.Generic;
using System.IO;

namespace PathfinderViewer
{
    class DataManager
    {
        #region private variables
        private IDictionary<string, DataFile> m_files;
        #endregion

        #region public constructors
        public DataManager()
        {
            m_files = new Dictionary<string, DataFile>();
        }
        #endregion

        #region public properties
        public ICollection<string> FileNames
        {
            get
            {
                return m_files.Keys;
            }
        }

        public DataFile this[string name]
        {
            get { return m_files[name]; }
        }
        #endregion

        #region public methods
        public void LoadFile(string name)
        {
            string filename = Path.GetFileName(name);
            DataFile file = new DataFile(filename);
            file.ReadFile(name);
            m_files[filename] = file;
        }

        public void GetBounds(string name, out double minvalue, out double maxvalue)
        {
            minvalue = double.MaxValue;
            maxvalue = double.MinValue;

            foreach(DataFile file in m_files.Values)
            {
                DataFile.DataFileElement elem = file[name];
                if (elem != null)
                {
                    if (elem.Minimum < minvalue)
                        minvalue = elem.Minimum;
                    if (elem.Maximum > maxvalue)
                        maxvalue = elem.Maximum;
                }
            }
        }
        #endregion
    }
}
