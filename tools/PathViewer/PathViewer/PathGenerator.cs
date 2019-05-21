using System;
using System.Collections.Generic;
using System.Threading;
using System.IO;
using Newtonsoft.Json;
using Csv;

namespace PathViewer
{
    public abstract class PathGenerator
    {
        #region private member variables
        /// <summary>
        /// Locks the queue for making changes
        /// </summary>
        private object m_lock;

        /// <summary>
        /// The queue of segment jobs to run
        /// </summary>
        private List<Tuple<RobotParams, RobotPath>> m_queue;

        /// <summary>
        /// The thread running the segment jobs
        /// </summary>
        private Thread m_segment_thread;

        /// <summary>
        /// If true, there is a job currently active
        /// </summary>
        private bool m_active;

        /// <summary>
        /// If true, the last event fired as zero running, zero queued
        /// </summary>
        private bool m_last_event_idle;

        /// <summary>
        /// If true, the background segment thread should be running
        /// </summary>
        private bool m_running;
        #endregion

        #region public events
        /// <summary>
        /// When the state of background segment generation jobs changes, this event is fired
        /// </summary>
        public event EventHandler<PathGenerationStateChangeEvent> StateChanged;
        #endregion

        #region public constructor
        public PathGenerator()
        {
            m_lock = new object();
            m_queue = new List<Tuple<RobotParams, RobotPath>>();
            m_running = true;
            m_active = false;
            m_last_event_idle = false;
            m_segment_thread = null;
        }
        #endregion

        #region public properties

        #endregion

        #region abstract methods and properties (must be implemented by derived class)
        public abstract string Name
        {
            get;
        }

        public abstract Version Version
        {
            get;
        }

        public abstract bool TimingConstraintsSupported
        {
            get;
        }

        public abstract void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines);

        public abstract PathSegment[] GenerateDetailedPath(RobotParams robot, RobotPath path);

        #endregion

        #region public methods

        public void Start()
        {
            m_running = true;
            m_segment_thread = new Thread(GenerateSegmentsThread);
            m_segment_thread.Name = Name;
            m_segment_thread.Start();
        }
        public void Stop()
        {
            if (m_running && m_segment_thread != null)
            {
                m_running = false;
                m_segment_thread.Join();
            }
        }

        public void GenerateSegments(RobotParams robot, RobotPath path)
        {
            PathGenerationStateChangeEvent args = null;
            Tuple<RobotParams, RobotPath> entry = new Tuple<RobotParams, RobotPath>(robot, path);
            lock (m_lock)
            {
                m_queue.Add(entry);
                int running = m_active ? 1 : 0;
                int queued = m_queue.Count;
                args = new PathGenerationStateChangeEvent(running, queued);
            }
            OnJobStateChanged(args);
        }

        #endregion

        #region protected methods
        protected void OnJobStateChanged(PathGenerationStateChangeEvent args)
        {
            EventHandler<PathGenerationStateChangeEvent> handler = StateChanged;
            handler?.Invoke(this, args);
        }

        protected bool GeneratePathFile(RobotParams robot, RobotPath path, string filename)
        {
            bool ret = true;
            PathFile pf = new PathFile();
            pf.Robot = robot;
            pf.AddPathGroup("tmp");
            pf.AddPath("tmp", path);

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

        protected PathSegment[] ParseOutputFile(string filename, string[] headers)
        {
            PathSegment[] ret = new PathSegment[0];

            string text = File.ReadAllText(filename);
            foreach (var line in CsvReader.ReadFromText(text))
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
        #endregion

        #region private methods

        private void GenerateSegmentsThread()
        {
            PathGenerationStateChangeEvent args = null;
            while (m_running)
            {
                Tuple<RobotParams, RobotPath> entry = null;

                lock (m_lock)
                {
                    if (m_queue.Count > 0)
                    {
                        entry = m_queue[0];
                        m_queue.RemoveAt(0);
                        m_active = true;
                    }
                    else
                    {
                        entry = null;
                        m_active = false;
                    }

                    args = null;
                    int running = m_active ? 1 : 0;
                    int queued = m_queue.Count;
                    if (running > 0 || queued > 0 || !m_last_event_idle)
                    {
                        args = new PathGenerationStateChangeEvent(running, queued);
                        if (running == 0 && queued == 0)
                            m_last_event_idle = true;
                        else
                            m_last_event_idle = false;
                    }
                }

                if (args != null)
                    OnJobStateChanged(args);

                if (entry != null)
                {
                    GenerateSegmentsForPath(entry.Item1, entry.Item2);
                }
                else
                {
                    Thread.Sleep(20);
                }
            }
        }

        private void GenerateSegmentsForPath(RobotParams robot, RobotPath path)
        {
            //
            // TODO - for the swerve drive, we need to look at the velocity and determine
            //        how much is allocated to rotation and how much is allocated to
            //        translation.
            //
            PathSegment[] segs = GenerateDetailedPath(robot, path);
            DriveModifier mod = null;

            if (robot.DriveType == RobotParams.TankDriveType)
            {
                mod = new TankDriveModifier();
            }
            else if (robot.DriveType == RobotParams.SwerveDriveType)
            {
                mod = new SwerveDriveModifier();
            }

            Dictionary<string, PathSegment[]> add = null;
            if (mod != null)
                add = mod.ModifyPath(robot, path, segs);

            path.SetSegments(segs, add);
        }

        private bool AddValue(PathSegment s, ICsvLine line, int index, string name)
        {
            double v;

            if (!Double.TryParse(line[index], out v))
                return false;

            s.SetValue(name, v);

            return true;
        }
        #endregion


    }
}
