using System;
using System.Collections.Generic;
using System.Text;
using PathViewer;

namespace PathFinderV1
{
    class Generator
    {
        private int m_count;
        private List<Spline> m_splines;
        private List<double> m_arclength;

        public Generator(int count)
        {
            m_count = count;
            m_splines = new List<Spline>();
            m_arclength = new List<double>();
        }

        public bool Prepare(WayPoint[] points)
        {
            if (points.Length < 2)
                return false;

            double total = 0.0;

            for(int i = 0; i < points.Length - 1; i++)
            {
                Spline s = fit(points[i], points[i + 1]);
                m_splines.Add(s);
                double dist = s.Distance(m_count);
                m_arclength.Add(dist);

                total += dist;
            }

            return true;
        }

        private Spline fit(WayPoint a, WayPoint b)
        {
            double aoffset = Math.Atan2(b.Y - a.Y, b.X - a.X);
            double a0_delta = Math.Tan(MathUtils.BoundRadians(a.Heading - aoffset)) ;
            double a1_delta = Math.Tan(MathUtils.BoundRadians(b.Heading - aoffset)) ;
            double knot = Math.Sqrt((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y));

            double va = -(3 * (a0_delta + a1_delta)) / (knot * knot * knot * knot);
            double vb = (8 * a0_delta + 7 * a1_delta) / (knot * knot * knot);
            double vc = -(6 * a0_delta + 4 * a1_delta) / (knot * knot);
            double vd = 0;
            double ve = a0_delta;

            Spline s = new Spline(va, vb, vc, vd, ve);
            s.KnotDistance = knot;
            s.AngleOffset = aoffset;
            s.XOffset = a.X;
            s.YOffset = a.Y;

            return s;
        }
    }
}
