using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace PathViewer
{
    public partial class FieldView : UserControl
    {
        #region private variables
        private double m_center_hit_dist = 8.0;
        private double m_tip_hit_dist = 6.0;
        private Game m_game;
        private Image m_image;
        private float m_image_scale;
        private Matrix m_world_to_window;
        private Matrix m_window_to_world;
        private RobotPath m_path;
        private WayPoint m_selected;
        private WayPoint m_rotating;
        private bool m_dragging;
        private float m_radius = 8.0f;
        private float m_velocity_scale = 1.0f;
        private PathFile m_file;
        private bool m_last_msg_selected;
        #endregion

        #region private enums
        private enum WaypointRegion
        {
            Center,
            Tip,
            None
        };
        #endregion

        #region public events

        /// <summary>
        /// This event is fired when a waypoint is selected with the mouse
        /// </summary>
        public event EventHandler<WaypointEventArgs> WaypointSelected;

        /// <summary>
        /// This event is fired when the X, Y, Heading, or Velocity of a waypoint changes
        /// </summary>
        public event EventHandler<WaypointEventArgs> WaypointChanged;

        //
        // This event is fired when the mouse is moved in the field window
        //
        public event EventHandler<FieldMouseMovedArgs> FieldMouseMoved;

        #endregion

        #region public constructors
        public FieldView()
        {
            m_path = null;
            m_world_to_window = new Matrix();
            m_window_to_world = new Matrix();
            m_selected = null;
            m_rotating = null;
            m_dragging = false;
            m_last_msg_selected = false;
            DoubleBuffered = true;

            InitializeComponent();
        }
        #endregion

        #region public properties

        public WayPoint SelectedWaypoint
        {
            get { return m_selected; }
            set { m_selected = value; Invalidate(); }
        }

        public Game FieldGame
        {
            get { return m_game; }
            set
            {
                m_game = value;
                if (m_game != null)
                {
                    m_image = Image.FromFile(m_game.ImagePath);
                    CalculateScale();
                }
                Invalidate();
            }
        }

        public RobotPath Path
        {
            get { return m_path ; }
            set { m_path = value; Invalidate(); }
        }

        public PathFile File
        {
            get { return m_file; }
            set
            {
                PathFile pf = m_file;

                m_selected = null;
                m_file = value;
                m_path = null;
            }
        }

        #endregion

        #region public methods
        #endregion

        #region overriden usercontrol methods

        protected void OnWayPointSelected(WaypointEventArgs args)
        {
            EventHandler<WaypointEventArgs> handler = WaypointSelected;
            handler?.Invoke(this, args);
        }

        protected void OnWayPointChanged(WaypointEventArgs args)
        {
            EventHandler<WaypointEventArgs> handler = WaypointChanged;
            handler?.Invoke(this, args);
        }

        protected void OnFieldMouseMoved(FieldMouseMovedArgs args)
        {
            EventHandler<FieldMouseMovedArgs> handler = FieldMouseMoved;
            handler?.Invoke(this, args);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            PointF pt = WindowToWorld(new PointF(e.X, e.Y));
            OnFieldMouseMoved(new FieldMouseMovedArgs(pt));

            PathGroup gr;
            RobotPath path;

            m_file.FindPathByWaypoint(m_selected, out gr, out path);

            if (m_dragging && m_selected != null)
            {
                if (m_last_msg_selected)
                {
                    if (m_selected != null)
                        OnWayPointChanged(new WaypointEventArgs(WaypointEventArgs.ReasonType.StartChange, gr, path, m_selected));

                    m_last_msg_selected = false;
                }

                InvalidateWaypoint(m_selected);
                m_selected.X = pt.X;
                m_selected.Y = pt.Y;
                InvalidateWaypoint(m_selected);

                OnWayPointChanged(new WaypointEventArgs(WaypointEventArgs.ReasonType.Changing, gr, path, m_selected));
            }
            else if (m_rotating != null)
            {
                if (m_last_msg_selected)
                {
                    if (m_selected != null)
                        OnWayPointChanged(new WaypointEventArgs(WaypointEventArgs.ReasonType.StartChange, gr, path, m_selected));

                    m_last_msg_selected = false;
                }

                m_rotating.Heading = FindAngle(new PointF((float)m_rotating.X, (float)m_rotating.Y), pt);
                InvalidateWaypoint(m_rotating);
                OnWayPointChanged(new WaypointEventArgs(WaypointEventArgs.ReasonType.Changing, gr, path, m_rotating));
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            PathGroup gr = null;
            RobotPath path = null;

            base.OnMouseUp(e);

            Capture = false;
            if (m_dragging)
            {
                m_file.FindPathByWaypoint(m_selected, out gr, out path);
                m_dragging = false;
                m_file.FindPathByWaypoint(m_selected, out gr, out path);
                OnWayPointChanged(new WaypointEventArgs(WaypointEventArgs.ReasonType.EndChange, gr, path, m_selected));
                Invalidate();
            }
            else if (m_rotating != null)
            {
                m_selected = m_rotating;
                m_rotating = null;
                InvalidateWaypoint(m_selected);
                m_file.FindPathByWaypoint(m_selected, out gr, out path);
                OnWayPointChanged(new WaypointEventArgs(WaypointEventArgs.ReasonType.EndChange, gr, path, m_selected));
                Invalidate();
            }
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            if (e.Button != MouseButtons.Left)
                return;

            WayPoint pt;
            WaypointRegion region;

            if (HitTestWaypoint(e.X, e.Y, out pt, out region))
            {
                PathGroup group;
                RobotPath path;

                Capture = true;

                m_file.FindPathByWaypoint(pt, out group, out path);
                if (region == WaypointRegion.Center)
                {
                    InvalidateWaypoint(m_selected);
                    InvalidateWaypoint(pt);
                    m_selected = pt;
                    m_dragging = true;
                    
                    OnWayPointSelected(new WaypointEventArgs(WaypointEventArgs.ReasonType.Selected, group, path, pt));
                    m_last_msg_selected = true;
                }
                else if (region == WaypointRegion.Tip)
                {
                    InvalidateWaypoint(m_selected);
                    m_selected = null;
                    m_rotating = pt;
                }
            }
            else
            { 
                InvalidateWaypoint(m_selected);
                m_selected = null;
            }
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            if (m_game != null)
                CalculateScale();

            Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            DrawBackground(e.Graphics);
            if (m_image != null)
                DrawField(e.Graphics);

            if (m_path != null)
                DrawPath(e.Graphics, m_path);
        }
        #endregion

        #region private methods

        private double FindAngle(PointF first, PointF second)
        {
            double angle = Math.Atan2(second.Y - first.Y, second.X - first.X);
            return angle / Math.PI * 180.0;
        }

        private bool HitTestWaypoint(int x, int y, out WayPoint hitpt, out WaypointRegion part)
        {
            bool found = false;
            hitpt = null;
            part = WaypointRegion.None;

            if (m_path == null)
                return false;

            //
            // See if there is a point near the point
            //
            foreach (WayPoint pt in m_path.Points)
            {
                PointF p = WorldToWindow(new PointF((float)pt.X, (float)pt.Y));

                //
                // Test the center of the waypoint
                //
                double dist = Math.Sqrt((p.X - x) * (p.X - x) + (p.Y - y) * (p.Y - y));
                if (dist < m_center_hit_dist)
                {
                    hitpt = pt;
                    part = WaypointRegion.Center;
                    found = true;
                    break;
                }

                //
                // Test the tip of the waypoint
                //
                PointF[] tip = new PointF[] { new PointF(2.0f, 0.0f) };

                Matrix mr = new Matrix();
                mr.Translate(p.X, p.Y);
                mr.Scale(12.0f, -12.0f);
                mr.Rotate((float)pt.Heading);
                mr.TransformPoints(tip);
                p = tip[0];

                dist = Math.Sqrt((p.X - x) * (p.X - x) + (p.Y - y) * (p.Y - y));
                if (dist < m_tip_hit_dist)
                {
                    hitpt = pt;
                    part = WaypointRegion.Tip;
                    found = true;
                    break;
                }
            }

            return found;
        }

        private void InvalidateWaypoint(WayPoint pt)
        {
            Invalidate();
        }

        private RectangleF WaypointBounds(WayPoint pt)
        {
            PointF p = WorldToWindow(new PointF((float)pt.X, (float)pt.Y));
            RectangleF r = new RectangleF(p.X, p.Y, 0.0f, 0.0f);
            r.Inflate(m_radius, m_radius);

            return r;
        }

        private void CalculateScale()
        {
            float sx = (float)Width / (float)m_image.Width;
            float sy = (float)Height / (float)m_image.Height;

            if (sx < sy)
                m_image_scale = sx;
            else
                m_image_scale = sy;

            m_world_to_window = new Matrix();
            float px = (float)m_game.FieldCorners.TopLeft[0] * m_image_scale;
            float py = (float)m_game.FieldCorners.BottomRight[1] * m_image_scale;
            m_world_to_window.Translate(px, py);

            //
            // Note, this scale between the field size in pixels in the image and the field size in inches
            // is the game for the X and Y dimensions, so we just use the one (x) in both places
            //
            sx = (float)m_game.FieldSize[0] / (float)(m_game.FieldCorners.BottomRight[0] - m_game.FieldCorners.TopLeft[0]);
            m_world_to_window.Scale(m_image_scale / sx, -m_image_scale / sx);

            float[] elems = m_world_to_window.Elements;
            m_window_to_world = new Matrix(elems[0], elems[1], elems[2], elems[3], elems[4], elems[5]);
            m_window_to_world.Invert();
        }

        private PointF WorldToWindow(PointF field)
        {
            PointF[] temp = new PointF[1] { new PointF(field.X, field.Y) };
            m_world_to_window.TransformPoints(temp);
            return temp[0];
        }

        private PointF WindowToWorld(PointF window)
        {
            PointF[] temp = new PointF[1] { new PointF(window.X, window.Y) };
            m_window_to_world.TransformPoints(temp);
            return temp[0];
        }

        private void DrawField(Graphics g)
        {
            RectangleF rect = new RectangleF(0.0f, 0.0f, m_image.Width * m_image_scale, m_image.Height * m_image_scale);
            g.DrawImage(m_image, rect);
        }

        private void DrawPath(Graphics g, RobotPath path)
        {
            if (path.HasSplines)
                DrawSplines(g, path);

            DrawPoints(g, path.Points);
        }

        private void DrawSplines(Graphics g, RobotPath path)
        {
            for (int i = 0; i < path.Splines.Length; i++)
                DrawSpline(g, path, i);
        }

        private void DrawSpline(Graphics g, RobotPath path, int index)
        {
            float diam = 2.0f;

            using (Brush b = new SolidBrush(Color.LightCoral))
            {
                double step = 1.0 / 1000.0;
                for (double t = 0.0; t < 1.0; t += step)
                {
                    double x, y, heading;
                    RectangleF rf;
                    double px, py;
                    PointF pt, w;

                    path.Evaluate(index, t, out x, out y, out heading);
                    heading = heading / 180.0 * Math.PI;
                    double ca = Math.Cos(heading);
                    double sa = Math.Sin(heading);

                    px = x - m_file.Robot.Width * sa / 2.0;
                    py = y + m_file.Robot.Width * ca / 2.0;

                    pt = new PointF((float)px, (float)py);
                    w = WorldToWindow(pt);

                    rf = new RectangleF(w, new SizeF(0.0f, 0.0f));
                    rf.Offset(diam / 2.0f, diam / 2.0f);
                    rf.Inflate(diam, diam);
                    g.FillEllipse(b, rf);

                    px = x + m_file.Robot.Width * sa / 2.0;
                    py = y - m_file.Robot.Width * ca / 2.0;

                    pt = new PointF((float)px, (float)py);
                    w = WorldToWindow(pt);

                    rf = new RectangleF(w, new SizeF(0.0f, 0.0f));
                    rf.Offset(diam / 2.0f, diam / 2.0f);
                    rf.Inflate(diam, diam);
                    g.FillEllipse(b, rf);
                }
            }
        }

        private void DrawPoints(Graphics g, WayPoint[] pts)
        {
            using (Pen sel = new Pen(Color.Yellow, 2.0f))
            {
                using (Pen raypen = new Pen(Color.LightBlue, 6.0f))
                {
                    using (Brush b = new SolidBrush(Color.Orange))
                    {
                        using (Pen pn = new Pen(Color.Black, 3.0f))
                        {
                            foreach (WayPoint pt in pts)
                            {
                                Matrix mm;

                                float angle = (float)(pt.Heading / 180.0f * Math.PI);

                                //
                                // Draw the point
                                //
                                PointF p = WorldToWindow(new PointF((float)pt.X, (float)pt.Y));

                                //
                                // Draw the heading, scaled by the velocity
                                //

                                PointF hp = new PointF((float)(Math.Cos(angle) * pt.Velocity * m_velocity_scale) + p.X,
                                                        -(float)(Math.Sin(angle) * pt.Velocity * m_velocity_scale) + p.Y);

                                g.DrawLine(raypen, p, hp);

                                //
                                // Draw the waypoint triangle
                                //
                                float tw = 10.0f;
                                PointF[] triangle = new PointF[] { new PointF(tw, 0.0f), new PointF(-tw / 2.0f, tw / 2.0f), new PointF(-tw /2.0f, -tw / 2.0f) };

                                mm = new Matrix();
                                mm.Translate((float)pt.X, (float)pt.Y);
                                mm.Rotate((float)pt.Heading);
                                mm.TransformPoints(triangle);
                                m_world_to_window.TransformPoints(triangle);

                                g.FillPolygon(b, triangle);
                                g.DrawPolygon(pn, triangle);

                                //
                                // Draw the selection box
                                //
                                if (m_selected == pt)
                                {
                                    PointF[] selbox = new PointF[]
                                    {
                                        new PointF((float)m_file.Robot.Width / 2.0f, (float)m_file.Robot.Length / 2.0f),
                                        new PointF(-(float)m_file.Robot.Width / 2.0f, (float)m_file.Robot.Length / 2.0f),
                                        new PointF(-(float)m_file.Robot.Width / 2.0f, -(float)m_file.Robot.Length / 2.0f),
                                        new PointF((float)m_file.Robot.Width / 2.0f, -(float)m_file.Robot.Length / 2.0f),
                                    };

                                    mm.TransformPoints(selbox);
                                    m_world_to_window.TransformPoints(selbox);
                                    g.DrawPolygon(sel, selbox);
                                }
                            }
                        }
                    }
                }
            }
        }

        private void DrawBackground(Graphics g)
        {
            using (Brush b = new SolidBrush(BackColor))
            {
                g.FillRectangle(b, 0, 0, Width, Height);
            }
        }
        #endregion
    }
}
