using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using XeroMath;


namespace PathViewer
{
    public partial class RobotFieldView : BasicFieldView
    {
        #region private variables
        private double m_time;
        private RobotParams m_robot;
        #endregion

        #region public constructors
        public RobotFieldView()
        {
            InitializeComponent();
        }

        #endregion

        #region public properties

        public RobotParams Robot
        {
            get { return m_robot; }
            set { m_robot = value; }
        }

        public double Time
        {
            get { return m_time; }
            set { m_time = value; Invalidate(); }
        }
        #endregion

        #region public methods
        #endregion

        #region overriden usercontrol methods
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (DisplayedPath != null)
            {
                DrawPathRobot(e.Graphics, DisplayedPath);
            }
        }

        protected override void OnPathChanged(EventArgs args)
        {
            base.OnPathChanged(args);
            Initialize();
        }
        #endregion

        #region private methods

        private void DrawPathRobot(Graphics g, RobotPath path)
        {
            if (path.AdditionalSegments == null)
                return;

            PointF[] pts = null;
            Dictionary<string, PathSegment[]> segs = path.AdditionalSegments;

            if (segs.Count == 2)
            {
                XeroPose left, right;
                left = path.GetPositionForTime(segs["left"], m_time);
                right = path.GetPositionForTime(segs["right"], m_time);

                XeroPose center = left.Interpolate(right, 0.5);

                pts = new PointF[]
                {
                    new PointF((float)Robot.Width / 2.0f, -(float)Robot.Length / 2.0f),
                    new PointF(-(float)Robot.Width / 2.0f, -(float)Robot.Length / 2.0f),
                    new PointF(-(float)Robot.Width / 2.0f, (float)Robot.Length / 2.0f),
                    new PointF((float)Robot.Width / 2.0f, (float)Robot.Length / 2.0f),
                };

                Matrix mm = new Matrix();
                mm.Translate((float)center.X, (float)center.Y);
                mm.Rotate((float)center.HeadingDegrees);
                mm.TransformPoints(pts);
                WorldToWindowMatrix.TransformPoints(pts);
            }
            else
            {
                XeroPose fl, fr, bl, br;
                fl = path.GetPositionForTime(segs["fl"], m_time);
                fr = path.GetPositionForTime(segs["fr"], m_time);
                bl = path.GetPositionForTime(segs["bl"], m_time);
                br = path.GetPositionForTime(segs["br"], m_time);

                pts = new PointF[4]
                {
                    new PointF((float)fr.X, (float)fr.Y),
                    new PointF((float)br.X, (float)br.Y),
                    new PointF((float)bl.X, (float)bl.Y),
                    new PointF((float)fl.X, (float)fl.Y),
                };

                WorldToWindowMatrix.TransformPoints(pts);
            }

            using (Pen p = new Pen(Color.Yellow, 3.0f))
            {
                g.DrawLines(p, pts);
            }

            using (Pen p = new Pen(Color.Green, 3.0f))
            {
                g.DrawLine(p, pts[3], pts[0]);
            }
        }

        private void Initialize()
        {
            m_time = 0.0;
        }
        #endregion
    }
}
