using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    class TrapezoidalProfile
    {
        #region private members
        private double m_start_velocity;
        private double m_end_velocity;
        private double m_actual_max_velocity;
        private double m_accel;
        private double m_max_velocity;
        private double m_tc;
        private double m_ta;
        private double m_td;
        private bool m_neg;
        private double m_distance;
        private string m_type;
        #endregion

        public TrapezoidalProfile(double accel, double maxvel)
        {
            m_accel = accel;
            m_max_velocity = maxvel;
        }

        public string TrapezoidalType
        {
            get { return m_type; }
        }

        public double TotalTime
        {
            get { return m_ta + m_tc + m_td; }
        }

        public void Update(double dist, double start, double end)
        {
            m_start_velocity = Math.Abs(start);
            m_end_velocity = Math.Abs(end);

            m_neg = (dist < 0);
            m_distance = Math.Abs(dist);
            m_ta = (m_max_velocity - m_start_velocity) / m_accel;
            m_td = (m_end_velocity - m_max_velocity) / -m_accel;
            double da = m_start_velocity * m_ta + 0.5 * m_accel * m_ta * m_ta;
            double dd = m_max_velocity * m_td + 0.5 * -m_accel * m_td * m_td;
            m_tc = (m_distance - da - dd) / m_max_velocity;
            m_type = "trapezoid";

            if (m_td < 0.0 || da + dd > m_distance)
            {
                //
                // We don't have time to get to the cruising velocity
                //
                double num = (-2.0 * m_distance * m_accel * m_accel + -m_accel * m_start_velocity * m_start_velocity - m_accel * m_end_velocity * m_end_velocity) / (-m_accel * 2);
                if (num < 0 || m_actual_max_velocity < m_start_velocity)
                {
                    // 
                    // Just decelerate down to the end
                    //
                    m_ta = 0;
                    m_tc = 0;
                    m_tc = (m_end_velocity - m_start_velocity) / -m_accel;
                    m_actual_max_velocity = m_start_velocity;
                    m_type = "line";
                }
                else
                {
                    //
                    // Can't get to max velocity but can accelerate some
                    // before decelerating
                    //
                    m_max_velocity = Math.Sqrt(num);
                    m_ta = (m_actual_max_velocity - m_start_velocity) / m_accel;
                    m_td = (m_end_velocity - m_actual_max_velocity) / -m_accel;
                    m_tc = 0;
                    m_type = "pyramid";
                }
            }
            else
            {
                //
                // Ok, now figure out the crusing time
                //
                m_actual_max_velocity = m_max_velocity;
                m_tc = (m_distance - da - dd) / m_max_velocity;
            }
        }

        public double GetAcceleration(double time)
        {
            double ret = 0.0;

            if (time < 0.0)
                ret = m_accel;
            else if (time < m_ta)
                ret = m_accel;
            else if (time < m_ta + m_tc)
                ret = 0.0;
            else if (time < m_ta + m_tc + m_td)
                ret = -m_accel;

            return m_neg ? -ret : ret ;
        }

        public double GetVelocity(double time)
        {
            double ret;
            if (time < 0.0)
            {
                ret = m_start_velocity;
            }
            else if (time < m_ta)
            {
                ret = m_start_velocity + time * m_accel;
            }
            else if (time < m_ta + m_tc)
            {
                ret = m_actual_max_velocity;
            }
            else if (time < m_ta + m_tc + m_td)
            {
                double dt = (time - m_ta - m_tc);
                ret = m_actual_max_velocity - dt * m_accel;
            }
            else
            {
                ret = m_end_velocity;
            }

            return m_neg ? -ret : ret;
        }

        public double GetDistance(double time)
        {
            double ret;

            if (time < 0.0)
            {
                ret = 0.0;
            }
            else if (time < m_ta)
            {
                ret = m_start_velocity * time + 0.5 * time * time * m_accel;
            }
            else if (time < m_ta + m_tc)
            {
                ret = m_start_velocity * m_ta + 0.5 * m_ta * m_ta * m_accel;
                ret += (time - m_ta) * m_actual_max_velocity;
            }
            else if (time < m_ta + m_tc + m_td)
            {
                double dt = time - m_ta - m_tc;
                ret = m_start_velocity * m_ta + 0.5 * m_ta * m_ta * m_accel;
                ret += m_tc * m_actual_max_velocity;
                ret += m_actual_max_velocity * dt - 0.5 * dt * dt * m_accel;
            }
            else
            {
                ret = m_distance;
            }

            return m_neg ? -ret : ret;
        }
    }
}
