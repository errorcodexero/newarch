using System;
using System.Collections.Generic;
using System.Text;

namespace XeroMath
{
    public class XeroPose
    {
        #region private members
        /// <summary>
        /// The position 
        /// </summary>
        private XeroVector m_vector;

        /// <summary>
        /// The heading in degrees
        /// </summary>
        private double m_heading;
        #endregion

        #region public constructor
        public XeroPose()
        {
            m_vector = new XeroVector();
            m_heading = 0.0;
        }

        public XeroPose(double x, double  y)
        {
            m_vector = new XeroVector(x, y);
            m_heading = 0.0;
        }

        public XeroPose(XeroVector v)
        {
            m_vector = new XeroVector(v.X, v.Y);
            m_heading = 0.0;
        }

        public XeroPose(XeroVector v, double heading)
        {
            m_vector = new XeroVector(v.X, v.Y);
            m_heading = heading;
        }

        public XeroPose(double x, double y , double heading)
        {
            m_vector = new XeroVector(x, y);
            m_heading = heading;
        }
        #endregion

        #region public properties
        public XeroVector Position
        {
            get { return m_vector; }
            set { m_vector = new XeroVector(value.X, value.Y); }
        }

        public double X
        {
            get { return m_vector.X; }
            set { m_vector.X = value; }
        }

        public double Y
        {
            get { return m_vector.Y; }
            set { m_vector.Y = value; }
        }

        public double HeadingDegrees
        {
            get { return m_heading; }
            set { m_heading = value; }
        }

        public double HeadingRadians
        {
            get { return XeroUtils.DegreesToRadians(m_heading); }
            set { m_heading = XeroUtils.RadiansToDegrees(value); }
        }
        #endregion

        #region public methods
        public XeroPose Interpolate(XeroPose other, double percent)
        {
            if (percent < 0.0)
                percent = 0.0;
            else if (percent > 1.0)
                percent = 1.0;

            XeroVector v = m_vector.Interpolate(other.Position, percent);
            double heading = m_heading + (other.m_heading - m_heading) * percent;

            return new XeroPose(v, heading);
        }

        public XeroPose RotateRadians(double radians)
        {
            XeroVector vector = m_vector.RotateRadians(radians);
            double heading = XeroUtils.BoundRadians(m_heading + radians);
            return new XeroPose(vector, heading);
        }

        public XeroPose RotateDegrees(double degrees)
        {
            return RotateRadians(XeroUtils.DegreesToRadians(degrees));
        }

        public XeroPose Translate(double dx, double dy)
        {
            return new XeroPose(X + dx, Y + dy, m_heading);
        }
        
        public XeroPose Translate(XeroVector v)
        {
            return Translate(v.X, v.Y);
        }

        #endregion

    }
}
