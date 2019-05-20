using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace PathViewer
{
    public partial class BasicFieldView : UserControl
    {
        #region private variables
        private Image m_image;
        private float m_image_scale;
        private Matrix m_world_to_window;
        private Matrix m_window_to_world;
        private RobotPath m_path;
        private Game m_game;
        #endregion

        #region
        //
        // This event is fired when the mouse is moved in the field window
        //
        public event EventHandler<FieldMouseMovedArgs> FieldMouseMoved;

        #endregion

        #region public constructors
        public BasicFieldView()
        {
            m_path = null;
            m_world_to_window = new Matrix();
            m_window_to_world = new Matrix();
            DoubleBuffered = true;

            InitializeComponent();
        }
        #endregion

        #region protected properties
        protected Matrix WorldToWindowMatrix
        {
            get { return m_world_to_window; }
        }

        protected Matrix WindowToWorldMatrix
        {
            get { return m_window_to_world; }
        }
        #endregion

        #region public properties

        public Game FieldGame
        {
            get { return m_game; }
            set
            {
                if (m_game != null)
                    m_game.UnitsChanged -= GameUnitsChanged;

                m_game = value;
                if (m_game != null)
                {
                    m_game.UnitsChanged += GameUnitsChanged;
                    m_image = Image.FromFile(m_game.ImagePath);
                    CalculateScale();
                }
                Invalidate();
                OnGameChanged(EventArgs.Empty);
            }
        }

        private void GameUnitsChanged(object sender, EventArgs e)
        {
            CalculateScale();
            Invalidate();
            OnGameChanged(EventArgs.Empty);
        }

        public RobotPath DisplayedPath
        {
            get { return m_path; }
            set { m_path = value; Invalidate(); OnPathChanged(EventArgs.Empty); }
        }
        #endregion

        #region public methods
        #endregion

        #region overriden usercontrol methods

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
        }
        #endregion

        #region protected methods
        protected virtual void OnGameChanged(EventArgs args)
        {
        }

        protected virtual void OnPathChanged(EventArgs args)
        {
        }
        #endregion

        #region private methods

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

        protected PointF WorldToWindow(PointF field)
        {
            PointF[] temp = new PointF[1] { new PointF(field.X, field.Y) };
            m_world_to_window.TransformPoints(temp);
            return temp[0];
        }

        protected PointF WindowToWorld(PointF window)
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
