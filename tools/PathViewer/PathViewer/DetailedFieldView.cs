using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace PathViewer
{
    public partial class DetailedFieldView : BasicFieldView
    {
        #region private variables
        private ViewTypeValue m_view_type;
        private double m_time;
        private RobotParams m_robot;

        private const int TimeStep = 100; 
        #endregion

        #region public enumeration
        public enum ViewTypeValue
        {
            WheelView,
            RobotView,
        };
        #endregion

        #region public constructors
        public DetailedFieldView()
        {
            InitializeComponent();
        }

        #endregion

        #region public properties
        public ViewTypeValue ViewType
        {
            get { return m_view_type; }
            set
            {
                m_view_type = value;
                Initialize();
                Invalidate();
            }
        }

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
                if (m_view_type == ViewTypeValue.WheelView)
                    DrawPathWheels(e.Graphics, DisplayedPath);
                else
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

        private void DrawOnePathSegment(Graphics g, Color c, PathSegment[] seg)
        {
            float radius = 2.0f;
            using (Brush b = new SolidBrush(c))
            {
                for (int i = 0; i < seg.Length; i++)
                {
                    double x = seg[i].GetValue("x");
                    double y = seg[i].GetValue("y");

                    PointF wpt = new PointF((float)x, (float)y);
                    PointF spt = WorldToWindow(wpt);

                    RectangleF r = new RectangleF(spt, new SizeF(0.0f, 0.0f));
                    r.Offset(-radius / 2.0f, -radius / 2.0f);
                    r.Inflate(radius, radius);
                    g.FillEllipse(b, r);
                }
            }
        }

        private Color[] m_colors = { Color.Red, Color.Yellow, Color.Green, Color.Pink };
        private void DrawPathWheels(Graphics g, RobotPath path)
        {
            if (path.AdditionalSegments == null)
                return;

            int i = 0;
            foreach(var pair in path.AdditionalSegments)
            {
                DrawOnePathSegment(g, m_colors[i++], pair.Value);
            }
        }

        private void DrawPathRobot(Graphics g, RobotPath path)
        {
            if (path.AdditionalSegments == null)
                return;

            PointF[] pts = null;

            Dictionary<string, PathSegment[]> segs = path.AdditionalSegments;

            if (segs.Count == 2)
            {
                double lx, ly, rx, ry;
                double heading;
                path.GetPositionForTime(segs["left"], m_time, out lx, out ly, out heading);
                path.GetPositionForTime(segs["right"], m_time, out rx, out ry, out heading);

                double x = (lx + rx) / 2.0;
                double y = (ly + ry) / 2.0;

                pts = new PointF[]
                {
                    new PointF((float)Robot.Width / 2.0f, -(float)Robot.Length / 2.0f),
                    new PointF(-(float)Robot.Width / 2.0f, -(float)Robot.Length / 2.0f),
                    new PointF(-(float)Robot.Width / 2.0f, (float)Robot.Length / 2.0f),
                    new PointF((float)Robot.Width / 2.0f, (float)Robot.Length / 2.0f),
                };

                Matrix mm = new Matrix();
                mm.Translate((float)x, (float)y);
                mm.Rotate((float)heading);
                mm.TransformPoints(pts);
                WorldToWindowMatrix.TransformPoints(pts);
            }
            else
            {
                double flx, fly, frx, fry;
                double blx, bly, brx, bry;
                double heading;
                path.GetPositionForTime(segs["fl"], m_time, out flx, out fly, out heading);
                path.GetPositionForTime(segs["fr"], m_time, out frx, out fry, out heading);
                path.GetPositionForTime(segs["bl"], m_time, out blx, out bly, out heading);
                path.GetPositionForTime(segs["br"], m_time, out brx, out bry, out heading);

                pts = new PointF[4]
                {
                    new PointF((float)frx, (float)fry),
                    new PointF((float)brx, (float)bry),
                    new PointF((float)blx, (float)bly),
                    new PointF((float)flx, (float)fly),
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
