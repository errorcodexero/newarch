using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace PathViewer
{
    public class GeneratorManager
    {
        #region private variables
        private List<PathGenerator> m_generators;
        #endregion

        #region public constructors
        public GeneratorManager()
        {
            m_generators = new List<PathGenerator>();
            SearchForGenerators();
        }
        #endregion

        #region public properties
        public IList<PathGenerator> Generators
        {
            get { return m_generators; }
        }

        public int Count
        {
            get { return m_generators.Count; }
        }
        #endregion

        #region public methods
        public void Clear()
        {
            m_generators.Clear();
        }
        #endregion

        #region private methods
        private void SearchForGenerators()
        {
            string dir = AppDomain.CurrentDomain.BaseDirectory;
            foreach (string file in Directory.GetFiles(dir, "*.dll"))
            {
                SearchForGenerators(file);
            }
        }

        private void SearchForGenerators(string file)
        {
            Type baseclass = typeof(PathGenerator);
            Assembly assembly = System.Reflection.Assembly.LoadFile(file);
            foreach(Type type in assembly.GetTypes())
            {
                if (type.IsClass && !type.IsAbstract && type.IsSubclassOf(baseclass))
                {
                    PathGenerator gen = (PathGenerator)Activator.CreateInstance(type);
                    m_generators.Add(gen);
                }
            }
        }

        #endregion

    }
}
