using System;

namespace PathViewer
{
    public class Spline
    {
        #region private 
        private double[] m_coeffs;

        private double m_knot_distance;
        private double m_angle_offset;
        private double m_x_offset;
        private double m_y_offset;
        #endregion

        public Spline(double dist, double[] coeffs)
        {
            m_coeffs = new double[coeffs.Length];
            Array.Copy(coeffs, m_coeffs, coeffs.Length);

            m_knot_distance = dist;
            m_angle_offset = 0.0;
            m_x_offset = 0.0;
            m_y_offset = 0.0;
        }

        public double KnotDistance
        {
            get { return m_knot_distance; }
            set { m_knot_distance = value; }
        }

        public double AngleOffset
        {
            get { return m_angle_offset; }
            set { m_angle_offset = value; }
        }

        public double XOffset
        {
            get { return m_x_offset; }
            set { m_x_offset = value; }
        }

        public double YOffset
        {
            get { return m_y_offset; }
            set { m_y_offset = value; }
        }

        public double Evaluate(double t)
        {
            double pow = 1.0;
            double sum = 0.0;

            for(int i = 0; i < m_coeffs.Length; i++)
            {
                sum += pow * m_coeffs[i];
                pow *= t;
            }

            return sum;
        }

        public double Derivative(double t)
        {
            double pow = 1.0;
            double sum = 0.0;

            for(int i = 1; i < m_coeffs.Length; i++)
            {
                sum += pow * m_coeffs[i] * i;
                pow *= t;
            }

            return sum;
        }
    }
}
