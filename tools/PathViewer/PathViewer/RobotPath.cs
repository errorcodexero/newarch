using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace PathViewer
{
    [JsonObject(MemberSerialization.OptIn)]
    public class RobotPath
    {
        #region public variables (mapped to JSON I/O)
        [JsonProperty(PropertyName = "name")]
        public string Name;

        [JsonProperty(PropertyName = "maxvelocity")]
        public double MaxVelocity;

        [JsonProperty(PropertyName = "maxacceleration")]
        public double MaxAcceleration;

        [JsonProperty(PropertyName = "startvelocity")]
        public double StartVelocity;

        [JsonProperty(PropertyName = "endvelocity")]
        public double EndVelocity;

        [JsonProperty(PropertyName = "constraints")]
        public TimingContraint[] Constraints;

        [JsonProperty(PropertyName = "points")]
        public WayPoint[] Points;
        #endregion

        #region public events
        public event EventHandler<EventArgs> SegmentsUpdated;
        #endregion

        #region private variables
        /// <summary>
        /// The splines for this path.  This is a list of X and Y splines that provide the equation to generate
        /// the path between two waypoints.  Therefore there should be N - 1 of these where N is the number of
        /// WayPoints.
        /// </summary>
        private Tuple<Spline, Spline>[] m_splines;

        /// <summary>
        /// The path segments for the fully generated path
        /// </summary>
        private PathSegment[] m_segments;

        /// <summary>
        /// If true, the segments are not up to date with the path
        /// </summary>
        private bool m_segments_dirty;

        /// <summary>
        /// the object to use as a lock
        /// </summary>
        private object m_lock;
        #endregion

        #region public constructors
        public RobotPath()
        {
            m_segments = null;
            m_splines = null;
            Points = new WayPoint[0];
            Name = string.Empty;
            m_lock = new object();
        }

        public RobotPath(string name)
        {
            m_segments = null;
            m_splines = null;
            Name = name;
            Points = new WayPoint[0];
            m_lock = new object();
        }

        public RobotPath(RobotPath path)
        {
            m_segments = null;
            m_splines = null;

            Name = path.Name;
            Points = new WayPoint[path.Points.Length];

            for (int i = 0; i < path.Points.Length; i++)
            {
                Points[i] = new WayPoint(path.Points[i]);
            }
            m_lock = new object();
        }
        #endregion

        #region public properties

        public bool HasSplines
        {
            get { return m_splines != null; }
        }

        public bool HasSegments
        {
            get { return m_segments != null && !m_segments_dirty; }
        }

        public Tuple<Spline, Spline> [] Splines
        {
            get
            {
                return m_splines;
            }
        }

        public PathSegment[] Segments
        {
            get { return m_segments; }
            set
            {
                lock (m_lock)
                {
                    m_segments = value;
                }
                OnSegmentsChanged(EventArgs.Empty);
            }
        }
        #endregion

        #region public methods
        public void SetSegmentsInvalid()
        {
            lock(m_lock)
            {
                m_segments_dirty = true;
            }
        }
        public void AddPoint(WayPoint pt)
        {
            Array.Resize<WayPoint>(ref Points, Points.Length + 1);
            Points[Points.Length - 1] = pt;
        }

        public bool InsertPoint(WayPoint pt, double vel)
        {
            int index = Array.IndexOf(Points, pt);
            if (index == -1 || index == Points.Length - 1)
                return false;

            WayPoint after = Points[index + 1];

            WayPoint newone = new WayPoint((pt.X + after.X) / 2, (pt.Y + after.Y) / 2, (pt.Heading + after.Heading) / 2, vel);
            Array.Resize(ref Points, Points.Length + 1);
            Array.Copy(Points, index, Points, index + 1, Points.Length - index - 1);
            Points[index + 1] = newone;
            return true;
        }

        public bool RemovePoint(WayPoint pt)
        {
            int index = Array.IndexOf(Points, pt);
            if (index == -1 || index == 0 || index == Points.Length - 1)
                return false;

            index++;
            while (index < Points.Length)
            {
                Points[index - 1] = Points[index];
                index++;
            }

            Array.Resize(ref Points, Points.Length - 1);
            return true;
        }

        public void ClearSplines()
        {
            m_splines = null;
        }

        public void GenerateSplines(PathGenerator gen)
        {
            List<Spline> xsp = new List<Spline>();
            List<Spline> ysp = new List<Spline>();

            gen.GenerateSplines(this, xsp, ysp);
            if (xsp.Count != ysp.Count)
                throw new Exception("Invalid path generator, GenerateSplines returned different number of X and Y splines");

            if (xsp.Count != Points.Length - 1)
                throw new Exception("Invalid path generator, GeneratedSplines did not return N-1 splines for N waypoints");

            m_splines = new Tuple<Spline, Spline>[xsp.Count];

            for(int i = 0; i < xsp.Count; i++)
            {
                Tuple<Spline, Spline> t = new Tuple<Spline, Spline>(xsp[i], ysp[i]);
                m_splines[i] = t;
            }
        }

        public void Evaluate(int index, double t, out double x, out double y, out double heading)
        {
            x = m_splines[index].Item1.Evaluate(t);
            y = m_splines[index].Item2.Evaluate(t);

            double dx = m_splines[index].Item1.Derivative(t);
            double dy = m_splines[index].Item2.Derivative(t);

            heading = Math.Atan2(dy, dx) / Math.PI * 180.0;
        }

        public void GenerateSegments(RobotParams robot, PathGenerator gen)
        {
            bool needgen = false;

            lock(m_lock)
            {
                if (m_segments == null || m_segments_dirty)
                {
                    needgen = true;
                    m_segments = null;
                    m_segments_dirty = false;
                }
            }

            if (needgen)
                gen.GenerateSegments(robot, this);
        }
        #endregion

        #region protected methods
        protected void OnSegmentsChanged(EventArgs args)
        {
            EventHandler<EventArgs> handler = SegmentsUpdated;
            handler?.Invoke(this, args);
        }
        #endregion
    }
}
