using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PathfinderViewer
{
    public partial class DoubleSlider : UserControl
    {
        #region event handler
        public event EventHandler ValueChanged;
        #endregion

        #region private enums
        private enum MarkerAlign
        {
            Left,
            Right
        };

        private enum DragMode
        {
            None,
            DraggingLeft,
            DraggingRight
        }
        #endregion

        #region private member variables
        private double m_minimum;
        private double m_maximum;
        private double m_left;
        private double m_right;
        private int m_ticks;
        private bool m_labels;

        private Color m_range_color;
        private Color m_normal_color;
        private Color m_tick_color;
        private Color m_label_color;
        private Color m_left_marker_fill_color;
        private Color m_right_marker_fill_color;
        private Color m_left_marker_outline_color;
        private Color m_right_marker_outline_color;

        private int m_bar_top;
        private int m_bar_height;
        private int m_bar_tick_spacing;
        private int m_tick_height;
        private int m_tick_label_spacing;
        private int m_marker_height;
        private int m_marker_width;
        private int m_marker_range_spacing;

        DragMode m_mode;

        #endregion

        #region public constructors
        public DoubleSlider()
        {
            m_mode = DragMode.None;

            m_minimum = 0.0;
            m_maximum = 100.0;
            m_left = 0.0;
            m_right = 100.0;
            m_ticks = 20;
            m_labels = true;

            m_bar_top = Font.Height;
            m_bar_height = 12;
            m_bar_tick_spacing = -m_bar_height;
            m_tick_height = 4 + m_bar_height;
            m_tick_label_spacing = 8;
            m_marker_width = 8;
            m_marker_height = 32;
            m_marker_range_spacing = 6;

            m_range_color = Color.LightGreen;
            m_normal_color = Color.LightBlue;
            m_tick_color = Color.DarkBlue;
            m_label_color = Color.Black;
            m_left_marker_fill_color = Color.CadetBlue;
            m_left_marker_outline_color = Color.Black;
            m_right_marker_fill_color = Color.CadetBlue;
            m_right_marker_outline_color = Color.Black;

            DoubleBuffered = true;
            InitializeComponent();
        }
        #endregion

        #region public properties
        public double Maximum
        {
            get { return m_maximum; }
            set
            {
                if (value <= m_minimum)
                    throw new Exception("invalid value for maximum, less than minimum");
                m_maximum = value;
                Invalidate();
            }
        }

        public double Minimum
        {
            get { return m_minimum; }
            set
            {
                if (value >= m_maximum)
                    throw new Exception("invalid value for minimum, greater than maximum");
                m_minimum = value;
                Invalidate();
            }
        }

        public double LeftValue
        {
            get { return m_left; }
            set
            {
                if (value > m_right)
                    throw new Exception("invalid value for left, greater than right");
                if (value < m_minimum)
                    m_left = m_minimum;
                else if (value > m_maximum)
                    m_left = m_maximum;
                else
                    m_left = value;

                Invalidate();
            }
        }

        public double RightValue
        {
            get { return m_right; }
            set
            {
                if (value < m_left)
                    throw new Exception("invalid value for right, less than left");
                if (value < m_minimum)
                    m_right = m_minimum;
                else if (value > m_maximum)
                    m_right = m_maximum;
                else
                    m_right = value;

                Invalidate();
            }
        }
        #endregion

        #region public methods
        #endregion

        #region protected methods
        protected virtual void OnValueChanged()
        {
            if (ValueChanged != null)
            {
                ValueChanged(this, EventArgs.Empty);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            DrawBar(e.Graphics);
            DrawTicks(e.Graphics);

            DrawMarker(e.Graphics, m_left, MarkerAlign.Right, m_left_marker_outline_color, m_left_marker_fill_color);
            DrawMarker(e.Graphics, m_right, MarkerAlign.Left, m_right_marker_outline_color, m_right_marker_fill_color);

            DrawRange(e.Graphics);
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            if (m_mode == DragMode.None)
            {
                if (GetMarkerBounds(m_left, MarkerAlign.Right).Contains(e.Location))
                {
                    m_mode = DragMode.DraggingLeft;
                    Capture = true;
                }
                else if (GetMarkerBounds(m_right, MarkerAlign.Left).Contains(e.Location))
                {
                    m_mode = DragMode.DraggingRight;
                    Capture = true;
                }
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (m_mode == DragMode.DraggingLeft)
            {
                using (Graphics g = CreateGraphics())
                {
                    Rectangle r1 = GetMarkerBounds(m_left, MarkerAlign.Right);
                    Rectangle r3 = GetLeftRegionBounds(g);

                    m_left = GetValue(e.Location.X);
                    if (m_left < m_minimum)
                        m_left = m_minimum;
                    else if (m_left > m_right)
                        m_left = m_right;

                    Rectangle r2 = GetMarkerBounds(m_left, MarkerAlign.Right);
                    r2.Inflate(m_marker_width * 2, Font.Height);
                    Rectangle r4 = GetLeftRegionBounds(g);
                    r4.Inflate(m_marker_width * 2, Height);

                    Invalidate(r1);
                    Invalidate(r2);
                    Invalidate(r3);
                    Invalidate(r4);

                    OnValueChanged();
                }
            }
            else if (m_mode == DragMode.DraggingRight)
            {
                using (Graphics g = CreateGraphics())
                {
                    Rectangle r1 = GetMarkerBounds(m_right, MarkerAlign.Left);
                    Rectangle r3 = GetRightRegionBounds(g);

                    m_right = GetValue(e.Location.X);
                    if (m_right > m_maximum)
                        m_right = m_maximum;
                    else if (m_right < m_left)
                        m_right = m_left;

                    Rectangle r2 = GetMarkerBounds(m_right, MarkerAlign.Left);
                    r2.Inflate(m_marker_width * 2, Font.Height);
                    Rectangle r4 = GetRightRegionBounds(g);
                    r4.Inflate(m_marker_width * 2, Font.Height);

                    Invalidate(r1);
                    Invalidate(r2);
                    Invalidate(r3);
                    Invalidate(r4);

                    OnValueChanged();
                }
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            Capture = false;
            m_mode = DragMode.None;
            Invalidate();
            OnValueChanged();
        }
        #endregion

        #region private methods

        #region position methods
        private int GetPixel(double v)
        {
            int width = Width - Margin.Left - Margin.Right;
            double pos = (v - m_minimum) / (m_maximum - m_minimum);
            return (int)(pos * width) + Margin.Left;
        }

        private double GetValue(int pixel)
        {
            double pos = ((double)pixel - (double)Margin.Left) / (double)(Width - Margin.Left - Margin.Right);
            return pos * (m_maximum - m_minimum);
        }

        Rectangle GetMarkerBounds(double value, MarkerAlign align)
        {
            int xloc = GetPixel(value);
            int yloc = Margin.Top + m_bar_top + m_bar_height / 2 - m_marker_height / 2;

            if (align == MarkerAlign.Right)
                xloc -= m_marker_width;

            return new Rectangle(xloc, yloc, m_marker_width, m_marker_height);
        }

        Rectangle GetLeftRegionBounds(Graphics g)
        {
            string label = string.Format("{0:F1}", m_left);
            int pos = GetPixel(m_left) + m_marker_range_spacing;
            SizeF s = g.MeasureString(label, Font);
            return new Rectangle(pos, Margin.Top, (int)s.Width , Font.Height);
        }

        Rectangle GetRightRegionBounds(Graphics g)
        {
            string label = string.Format("{0:F1}", m_right);
            SizeF s = g.MeasureString(label, Font);
            int pos = GetPixel(m_right) - m_marker_range_spacing - (int)s.Width;
            return new Rectangle(pos, Margin.Top, (int)s.Width, Font.Height);
        }
        #endregion

        #region drawing methods
        private void DrawBar(Graphics g)
        {
            using (Brush b = new SolidBrush(m_normal_color))
            {
                int width = GetPixel(m_left) - Margin.Left;
                Rectangle r = new Rectangle(Margin.Left, Margin.Top + m_bar_top, width, m_bar_height);
                g.FillRectangle(b, r);

                width = Width - Margin.Right - GetPixel(m_right);
                r = new Rectangle(GetPixel(m_right), Margin.Top + m_bar_top, width, m_bar_height);
                g.FillRectangle(b, r);
            }

            using (Brush b = new SolidBrush(m_range_color))
            {
                int width = GetPixel(m_right) - GetPixel(m_left);
                Rectangle r = new Rectangle(GetPixel(m_left), Margin.Top + m_bar_top, width, m_bar_height);
                g.FillRectangle(b, r);
            }
        }

        private void DrawTicks(Graphics g)
        {
            if (m_ticks <= 1)
                return;

            using (Brush b = new SolidBrush(m_label_color))
            {
                using (Pen p = new Pen(m_tick_color))
                {
                    double each = (m_maximum - m_minimum) / (m_ticks - 1);
                    int top = Margin.Top + m_bar_top + m_bar_height + m_bar_tick_spacing;
                    for (int i = 0; i < m_ticks; i++)
                    {
                        int pos = GetPixel(m_minimum + i * each);
                        g.DrawLine(p, pos, top, pos, top + m_tick_height);

                        if (m_labels)
                        {
                            string label = string.Format("{0:F1}", GetValue(pos));
                            SizeF size = g.MeasureString(label, Font);
                            float left = pos - size.Width / 2.0f;
                            float baseline = top + m_tick_height + m_tick_label_spacing;

                            g.DrawString(label, Font, b, left, baseline);
                        }
                    }
                }
            }
        }

        private void DrawMarker(Graphics g, double value, MarkerAlign align, Color outline, Color fill)
        {
            Rectangle r = GetMarkerBounds(value, align);

            using (Brush b = new SolidBrush(fill))
            {
                g.FillRectangle(b, r);
            }

            using (Pen p = new Pen(outline))
            {
                g.DrawRectangle(p, r);
            }
        }

        private void DrawRange(Graphics g)
        {
            float pos;
            string label;

            using (Brush b = new SolidBrush(m_label_color))
            {
                label = string.Format("{0:F1}", m_left);
                pos = GetPixel(m_left) + m_marker_range_spacing;
                g.DrawString(label, Font, b, pos, Margin.Top);

                label = string.Format("{0:F1}", m_right);
                SizeF s = g.MeasureString(label, Font);
                pos = GetPixel(m_right) - m_marker_range_spacing - s.Width;
                g.DrawString(label, Font, b, pos, Margin.Top);
            }
        }
        #endregion
        #endregion
    }
}

