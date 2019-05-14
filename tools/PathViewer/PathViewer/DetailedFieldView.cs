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
        #endregion

        #region public constructors
        public DetailedFieldView()
        {
            InitializeComponent();
        }
        #endregion

        #region public properties
        #endregion

        #region public methods
        #endregion

        #region overriden usercontrol methods

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (DisplayedPath != null)
                DrawPath(e.Graphics, DisplayedPath);
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
        private void DrawPath(Graphics g, RobotPath path)
        {
            if (path.AdditionalSegments == null)
                return;

            int i = 0;
            foreach(var pair in path.AdditionalSegments)
            {
                DrawOnePathSegment(g, m_colors[i++], pair.Value);
            }
        }
        #endregion
    }
}
