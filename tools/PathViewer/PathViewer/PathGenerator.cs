using System;
using System.Collections.Generic;
using System.Threading;
using System.IO;
using System.Diagnostics;
using Newtonsoft.Json;
using Csv;

namespace PathViewer
{
    public abstract class PathGenerator
    {
        #region private member variables
        private object m_lock;
        private Dictionary<RobotPath, Thread> m_paths;
        private int m_serial;
        #endregion

        #region public constructor
        public PathGenerator()
        {
            m_lock = new object();
            m_paths = new Dictionary<RobotPath, Thread>();
            m_serial = 0;
        }
        #endregion

        #region public properties
        public abstract string Name
        {
            get;
        }

        public abstract Version Version
        {
            get;
        }
        #endregion

        public abstract void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines);

        public abstract PathSegment[] GenerateDetailedPath(RobotParams robot, RobotPath path);

        public void GenerateSegments(RobotParams robot, RobotPath path)
        {
            lock (m_lock)
            {
                //
                // See if we are already generating the detailed in the background.  If so,
                // we don't start anything else until it is done
                //
                if (m_paths.ContainsKey(path))
                {
                    return;
                }

                Thread th = new Thread(GenerateSegmentsThread);

                m_paths.Add(path, th);
                th.Start(new Tuple<RobotParams, RobotPath>(robot, path));
            }
        }

        public bool IsGenerating(RobotPath path)
        {
            bool ret = false;

            lock(m_lock)
            {
                ret = m_paths.ContainsKey(path);
            }

            return ret;
        }

        private void GenerateSegmentsThread(object opath)
        {
            Tuple<RobotParams, RobotPath> data = opath as Tuple<RobotParams, RobotPath>;
            PathSegment[] segs = GenerateDetailedPath(data.Item1, data.Item2);

            lock(m_lock)
            {
                Debug.WriteLine("  Finished generation '" + data.Item2.Name + "' - removing from map");
                m_paths.Remove(data.Item2);
                data.Item2.Segments = segs;
            }
        }

        protected bool GeneratePathFile(RobotParams robot, RobotPath path, string filename)
        {
            bool ret = true;
            PathFile pf = new PathFile();
            pf.Robot = robot;
            pf.AddPathGroup("tmp");
            pf.AddPath("tmp", path);
            m_serial++;

            string json = JsonConvert.SerializeObject(pf);
            try
            {
                File.WriteAllText(filename, json);
            }
            catch (Exception)
            {
                ret = false;
            }

            return ret;
        }

        private bool AddValue(PathSegment s, ICsvLine line, int index, string name)
        {
            double v;

            if (!Double.TryParse(line[index], out v))
                return false;

            s.AddValue(name, v);

            return true;
        }

        protected PathSegment[] ParseOutputFile(string filename, string [] headers)
        {
            PathSegment[] ret = new PathSegment[0];

            string text = File.ReadAllText(filename);
            foreach(var line in CsvReader.ReadFromText(text))
            {
                PathSegment ps = new PathSegment();
                if (line.ColumnCount != headers.Length)
                    continue;

                for (int i = 0; i < headers.Length; i++)
                    AddValue(ps, line, i, headers[i]);

                Array.Resize<PathSegment>(ref ret, ret.Length + 1);
                ret[ret.Length - 1] = ps;
            }

            return ret;
        }
    }
}
