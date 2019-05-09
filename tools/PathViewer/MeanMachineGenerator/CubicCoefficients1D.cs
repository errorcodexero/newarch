using System;

namespace MeanMachineGenerator
{
    public class CubicCoefficients1D
    {
        #region private member variables
        private double m_a;
        private double m_b;
        private double m_c;
        private double m_d;
        private int m_steps;
        private double m_fda;
        private double m_fdb;
        private double m_fdc;
        private double m_fdd;
        private double m_prev_value;
        #endregion

        #region public constructors
        public CubicCoefficients1D()
        {
            m_steps = 0;
            m_a = 0.0;
            m_b = 0.0;
            m_c = 0.0;
            m_d = 0.0;
        }

        public CubicCoefficients1D(double p1, double p4, double r1, double r4)
        {
            m_a = 2 * p1 - 2 * p4 + r1 + r4;
            m_b = -3 * p1 + 3 * p4 - 2 * r1 - r4;
            m_c = r1;
            m_d = p1;
        }
        #endregion

        #region public properties
        public int Steps
        {
            get { return m_steps; }
        }

        public double FDA
        {
            get { return m_fda; }
        }

        public double PreviousFDA
        {
            get { return m_prev_value; }
        }
        #endregion

        #region public methods
        public double InitFD(int steps)
        {
            m_steps = steps;

            double fd12 = 1.0 / (double)m_steps;
            double fd11 = fd12 * fd12;
            double fd10 = fd11* fd12;
            double fd20 = 6.0 * fd10;
            double fd21 = 2.0 * fd11;

            m_fda = m_d;
            m_fdb = m_a * fd10 + m_b * fd11 + m_c * fd12;
            m_fdc = m_a * fd20 + m_b * fd21;
            m_fdd = m_a * fd20;

            return m_fda;
        }

        public double Evaluate(double t)
        {
            return t * (t * (m_a * t + m_b) + m_c) + m_d;
        }

        public double FirstDerivative(double t)
        {
            return t * (3 * m_a * t + 2 * m_b) + m_c;
        }

        public double SecondDerivative(double t)
        {
            return 6 * m_a * t + 2 * m_b;
        }

        public double BumpFD()
        {
            m_prev_value = m_fda;
            m_fda += m_fdb;
            m_fdb += m_fdc;
            m_fdc += m_fdd;

            return m_fda;
        }

        public double BumpFDFaster()
        {
            m_fda += m_fdb;
            m_fdb += m_fdc;
            m_fdc += m_fdd;

            return m_fda;
        }
        #endregion
    }
}
