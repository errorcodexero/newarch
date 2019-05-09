using System;
using System.Collections.Generic;
using System.Text;

namespace MeanMachineGenerator
{
    class MeanVector2
    {
        #region private variables
        private double m_x;
        private double m_y;
        #endregion

        #region public constructors

        public MeanVector2()
        {
            m_x = 0.0;
            m_y = 0.0;

        }
        public MeanVector2(double x, double y)
        {
            m_x = x;
            m_y = y;
        }
        #endregion

        #region public properties
        public double X
        {
            get { return m_x; }
            set { m_x = value; }
        }

        public double Y
        {
            get { return m_y; }
            set { m_y = value; }
        }
        #endregion

        #region public static methods
        public static MeanVector2 Add(MeanVector2 a, MeanVector2 b)
        {
            return new MeanVector2(a.X + b.X, a.Y + b.Y);
        }

        public static MeanVector2 Subtract(MeanVector2 a, MeanVector2 b)
        {
            return new MeanVector2(a.X - b.X, a.Y - b.Y);
        }

        public static MeanVector2 Multiply(MeanVector2 a, double b)
        {
            return new MeanVector2(a.X * b, a.Y * b);
        }

        public static MeanVector2 Divide(MeanVector2 a, double b)
        {
            return new MeanVector2(a.X / b, a.Y / b);
        }
        #endregion
    }
}
