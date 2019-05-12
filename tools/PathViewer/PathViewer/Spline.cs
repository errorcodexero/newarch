using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    public class Spline
    {
        #region private 
        private double[] m_coeffs;
        #endregion

        public Spline(double [] coeffs)
        {
            m_coeffs = new double[coeffs.Length];
            Array.Copy(coeffs, m_coeffs, coeffs.Length);
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
