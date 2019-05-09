using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace MeanMachineGenerator
{
    class MotionKey
    {
        #region enums
        public enum SlopeMethod
        {
            SLOPE_MANUAL, SLOPE_LINEAR, SLOPE_FLAT, SLOPE_SMOOTH, SLOPE_CLAMPED, SLOPE_PLATEAU,
            SLOPE_STEPPED, SLOPE_STEPPED_NEXT
        };
        #endregion

        #region private constants
        private const double CLAMPTOLERANCE = 0.005;
        private const double EPSILON = 1E-8;
        #endregion

        #region private variables

        private MeanVector2 m_timeAndValue;
        private MeanVector2 m_prevAngleAndMagnitude;
        private MeanVector2 m_nextAngleAndMagnitude;
        private MeanVector2 m_prevTangent;
        private MeanVector2 m_nextTangent;
        private SlopeMethod m_prevSlopeMethod;
        private SlopeMethod m_nextSlopeMethod;
        private bool m_markBeginOrEndKeysToZeroSlope;
        private MotionKey m_nextKey;
        private MotionCurve m_motionCurve;
        private  MotionKey m_prevKey;
        private  bool m_bTangentsDirty;
        private  bool m_bCoefficientsDirty;
        private  CubicCoefficients1D m_xCoeff;
        private  CubicCoefficients1D m_yCoeff;
        #endregion

        #region public constructors
        public MotionKey()
        {
            m_timeAndValue = new MeanVector2(0.0, 0.0);
            m_prevAngleAndMagnitude = new MeanVector2(0.0, 1.0);
            m_nextAngleAndMagnitude = new MeanVector2(0.0, 1.0);
            m_prevTangent = new MeanVector2(0.0, 0.0);
            m_nextTangent = new MeanVector2(0.0, 0.0);

            m_bTangentsDirty = true;
            m_bCoefficientsDirty = true;

            m_prevSlopeMethod = SlopeMethod.SLOPE_SMOOTH;
            m_nextSlopeMethod = SlopeMethod.SLOPE_SMOOTH;
            m_markBeginOrEndKeysToZeroSlope = true;
            m_nextKey = null;
            m_prevKey = null;
        }
        #endregion

        #region public properties

        public MotionCurve Curve
        {
            get { return m_motionCurve; }
            set { m_motionCurve = value; }
        }

        public double Time
        {
            get { return m_timeAndValue.X; }
            set { m_timeAndValue.X = value; }
        }

        public double Value
        {
            get { return m_timeAndValue.Y; }
            set { m_timeAndValue.Y = value; }
        }

        public double PrevMagnitude
        {
            get { return m_prevAngleAndMagnitude.Y; }
            set
            {
                m_prevAngleAndMagnitude.Y = value;
                m_prevSlopeMethod = SlopeMethod.SLOPE_MANUAL;
            }
        }

        public double NextMagnitude
        {
            get { return m_nextAngleAndMagnitude.Y; }
            set
            {
                m_nextAngleAndMagnitude.Y = value;
                m_nextSlopeMethod = SlopeMethod.SLOPE_MANUAL;
            }
        }

        public double PrevAngle
        {
            get { return m_prevAngleAndMagnitude.X; }
            set
            {
                m_prevAngleAndMagnitude.X = value;
                m_prevSlopeMethod = SlopeMethod.SLOPE_MANUAL;
            }
        }

        public double NextAngle
        {
            get { return m_nextAngleAndMagnitude.X; }
            set
            {
                m_nextAngleAndMagnitude.X = value;
                m_nextSlopeMethod = SlopeMethod.SLOPE_MANUAL;
            }
        }

        public MotionKey NextKey
        {
            get { return m_nextKey; }
            set { m_nextKey = value; }
        }

        public MotionKey PrevKey
        {
            get { return m_prevKey; }
            set { m_prevKey = value; }
        }

        public SlopeMethod PrevSlopeMethod
        {
            get { return m_prevSlopeMethod; }
            set { m_prevSlopeMethod = value; }
        }

        public SlopeMethod NextSlopeMethod
        {
            get { return m_nextSlopeMethod; }
            set { m_nextSlopeMethod = value; }
        }

        public bool TangentsDirty
        {
            get { return m_bTangentsDirty; }
            set { m_bTangentsDirty = value; }
        }

        public bool CoefficientsDirty
        {
            get { return m_bCoefficientsDirty; }
            set { m_bCoefficientsDirty = value; }
        }

        public MeanVector2 TimeValue
        {
            get { return m_timeAndValue; }
        }

        public MeanVector2 NextAngleAndMagnitude
        {
            get { return m_nextAngleAndMagnitude; }
            set
            {
                m_markBeginOrEndKeysToZeroSlope = false;
                m_nextAngleAndMagnitude = value;
                m_prevSlopeMethod = SlopeMethod.SLOPE_MANUAL;
                TangentsDirty = true;
                OnPositionChanged();
            }
        }

        public MeanVector2 PrevAngleAndMagnitude
        {
            get { return m_prevAngleAndMagnitude; }
            set
            {
                m_markBeginOrEndKeysToZeroSlope = false;
                m_prevAngleAndMagnitude = value;
                m_prevSlopeMethod = SlopeMethod.SLOPE_MANUAL;
                TangentsDirty = true;
                OnPositionChanged();
            }
        }

        public CubicCoefficients1D XCoefficients
        {
            get
            {
                if (m_bCoefficientsDirty)
                    calculateCoefficients();

                return m_xCoeff;
            }
        }

        public CubicCoefficients1D YCoefficients
        {
            get
            {
                if (m_bCoefficientsDirty)
                    calculateCoefficients();

                return m_yCoeff;
            }
        }

        public MeanVector2 NextTangent
        {
            get
            {
                if (m_bTangentsDirty)
                    calculateTangents();

                return m_nextTangent;
            }
        }

        public MeanVector2 PrevTangent
        {
            get
            {
                if (m_bTangentsDirty)
                    calculateTangents();

                return m_prevTangent;
            }
        }

        public bool IsMarkBeginOrEndKeysToZeroSlope
        {
            get { return m_markBeginOrEndKeysToZeroSlope; }
            set { m_markBeginOrEndKeysToZeroSlope = value; }
        }

        public double Magnitude
        {
            get
            {
                if (m_prevKey != null)
                    return PrevMagnitude;

                return NextMagnitude;
            }
            set
            {
                m_prevAngleAndMagnitude.Y = value;
                m_nextAngleAndMagnitude.Y = value;
                m_nextSlopeMethod = SlopeMethod.SLOPE_MANUAL;
                m_prevSlopeMethod = SlopeMethod.SLOPE_MANUAL;
                OnPositionChanged();
            }
        }

        public double Angle
        {
            get
            {
                if (m_prevKey != null)
                    return PrevAngle;

                return NextAngle;
            }
            set
            {
                m_prevAngleAndMagnitude.X = value;
                m_nextAngleAndMagnitude.X = value;
                m_nextSlopeMethod = SlopeMethod.SLOPE_MANUAL;
                m_prevSlopeMethod = SlopeMethod.SLOPE_MANUAL;
                OnPositionChanged();
            }
        }
        #endregion

        #region public methods
        public void InsertBefore(MotionKey key)
        {
            m_prevKey = key.PrevKey;
            if (key.m_prevKey != null)
                key.m_prevKey.m_nextKey = this;
            key.m_prevKey = this;
            m_nextKey = key;
        }

        public void InsertAfter(MotionKey key)
        {
            m_nextKey = key.m_nextKey;
            if (key.m_nextKey != null)
                key.m_nextKey.m_prevKey = this;
            key.m_nextKey = this;
            m_prevKey = key;
        }

        public void OnPositionChanged()
        {
            if (m_motionCurve != null)
                m_motionCurve.OnKeyPositionChanged(this);

            TangentsDirty = true;
            CoefficientsDirty = true;

            if (m_prevKey != null)
            {
                m_prevKey.TangentsDirty = true;
                m_prevKey.CoefficientsDirty = true;

                if (m_prevKey.m_prevKey != null && m_prevKey.m_prevKey.m_nextSlopeMethod == SlopeMethod.SLOPE_PLATEAU)
                {
                    m_prevKey.m_prevKey.CoefficientsDirty = true;
                    m_prevKey.m_prevKey.TangentsDirty = true;
                }
            }

            if (m_nextKey != null)
            {
                m_nextKey.TangentsDirty = true;
                if (m_nextKey.m_nextKey != null && m_nextKey.m_nextKey.m_prevSlopeMethod == SlopeMethod.SLOPE_PLATEAU)
                {
                    m_nextKey.m_nextKey.TangentsDirty = true ;
                    m_nextKey.CoefficientsDirty = true;
                }
            }
        }
        #endregion

        #region private methods
        private void calculateCoefficients()
        {
            m_bCoefficientsDirty = false;

            double pointax = Time;
            double pointbx = m_nextKey.Time;
            double xspan = pointbx - pointax;

            double pointay = Value;
            double pointby = m_nextKey.Value;
            double pointcy = NextTangent.Y;
            double pointdy = m_nextKey.PrevTangent.Y;

            m_yCoeff = new CubicCoefficients1D(pointay, pointby, pointcy, pointdy);

            // if the weights are default, then the x cubic is linear and there is no need to evaluate it
            if (Math.Abs(NextMagnitude - 1.0) < EPSILON && Math.Abs(m_nextKey.PrevMagnitude - 1.0) < EPSILON)
                return;

            // Spline - non default tangents means that we need a second parametric cubic for x as a function of t
            double pointcx = NextTangent.X;
            double pointdx = m_nextKey.PrevTangent.X;

            double xspan3 = xspan * 3;

            // if c going beyond b limit
            if (pointcx > xspan3)
            {
                double ratio = xspan3 / pointcx;
                pointcx = xspan3;
            }

            // if d going beyond a limit
            if (pointdx > xspan3)
            {
                double ratio = xspan3 / pointdx;
                pointdx = xspan3;
            }

            m_xCoeff = new CubicCoefficients1D(pointax, pointbx, pointcx, pointdx);
        }

        private void calculateTangents()
        {
            m_bTangentsDirty = false;


            bool bCalcSmoothPrev = false;
            bool bCalcSmoothNext = false;

            switch (m_prevSlopeMethod)
            {
                case SlopeMethod.SLOPE_MANUAL:
                    m_prevTangent.X = Math.Cos(m_prevAngleAndMagnitude.X);
                    m_prevTangent.Y = Math.Sin(m_prevAngleAndMagnitude.X);
                    if (m_prevKey != null)
                        MeanVector2.Multiply(m_prevTangent, Time - m_prevKey.Time);
                    break;
                case SlopeMethod.SLOPE_LINEAR:
                    if (m_prevKey != null)
                        m_prevTangent = MeanVector2.Subtract(TimeValue, m_prevKey.TimeValue);
                    break;
                case SlopeMethod.SLOPE_FLAT:
                    if (m_prevKey != null)
                    {
                        m_prevTangent.X = (Time - m_prevKey.Time) * 0.5;
                        m_prevTangent.Y = 0.0;
                    }
                    break;
                case SlopeMethod.SLOPE_SMOOTH:
                    bCalcSmoothPrev = true;
                    break;
                case SlopeMethod.SLOPE_CLAMPED:
                    {
                        double fClampTolerence = (m_motionCurve.MaxValue - m_motionCurve.MinValue) * CLAMPTOLERANCE;
                        if (m_prevKey != null && Math.Abs(m_prevKey.Value - Value) <= fClampTolerence) // make Flat
                        {
                            m_prevTangent.X = Time - m_prevKey.Time;
                            m_prevTangent.Y = 0.0;
                        }
                        else if (m_nextKey != null && Math.Abs(m_nextKey.Value - Value) <= fClampTolerence) // Make Flat
                        {
                            if (m_prevKey != null)
                            {
                                m_prevTangent.X = Time - m_prevKey.Time;
                                m_prevTangent.Y = 0.0;
                            }
                            else
                            {
                                m_prevTangent.X = 0.0;
                                m_prevTangent.Y = 0.0;
                            }
                        }
                        else
                            bCalcSmoothPrev = true;
                        break;
                    }
                case SlopeMethod.SLOPE_PLATEAU:
                    if (m_prevKey == null || m_nextKey == null)
                    {
                        if (m_prevKey != null)
                        {
                            m_prevTangent.X = Time - m_prevKey.Time;
                            m_prevTangent.Y = 0.0;
                        }
                        else
                        {
                            m_prevTangent.X = 0.0;
                            m_prevTangent.Y = 0.0;
                        }
                    }
                    else // we have a prev and a next, lets see if both the prev's out tangent and the next's in tangent are both either greater or less than our value, if so lets make out tangent flat
                    {
                        double fPrevTangentValue;
                        if (m_prevKey.m_nextSlopeMethod == SlopeMethod.SLOPE_PLATEAU)
                            fPrevTangentValue = m_prevKey.Value;
                        else
                        {
                            MeanVector2 vPrevPos = m_prevKey.TimeValue;
                            MeanVector2 vPrevTangent =MeanVector2.Add(MeanVector2.Multiply(m_prevKey.NextTangent, 1.0 / 3.0), vPrevPos);
                            fPrevTangentValue = vPrevTangent.Y;
                        }

                        double fNextTangentValue;
                        if (m_nextKey.m_prevSlopeMethod == SlopeMethod.SLOPE_PLATEAU)
                            fNextTangentValue = m_nextKey.Value;
                        else
                        {
                            MeanVector2 vNextPos = m_nextKey.TimeValue;
                            MeanVector2 vNextTangent = MeanVector2.Subtract(vNextPos, MeanVector2.Multiply(m_nextKey.PrevTangent, 1.0 / 3.0));
                            fNextTangentValue = vNextTangent.Y;
                        }

                        double fValue = Value;
                        if (fPrevTangentValue > fValue && fNextTangentValue > fValue)
                        {
                            m_prevTangent.X = Time - m_prevKey.Time;
                            m_prevTangent.Y = 0;
                        }
                        else if (fPrevTangentValue < fValue && fNextTangentValue < fValue)
                        {
                            m_prevTangent.X = Time - m_prevKey.Time;
                            m_prevTangent.Y = 0.0;
                        }
                        else
                            bCalcSmoothPrev = true;
                    }
                    break;
                case SlopeMethod.SLOPE_STEPPED:
                case SlopeMethod.SLOPE_STEPPED_NEXT:
                    Debug.Assert(false);
                    break;
            }

            switch (m_nextSlopeMethod)
            {
                case SlopeMethod.SLOPE_MANUAL:
                    m_nextTangent.X = Math.Cos(NextAngle);
                    m_nextTangent.Y = Math.Sin(NextAngle);
                    if (m_nextKey != null)
                        MeanVector2.Multiply(m_nextTangent, m_nextKey.Time - Time);
                    break;
                case SlopeMethod.SLOPE_LINEAR:
                    if (m_nextKey != null)
                        m_nextTangent = MeanVector2.Subtract(m_nextKey.TimeValue, TimeValue);
                    break;
                case SlopeMethod.SLOPE_FLAT:
                    if (m_nextKey != null)
                    {
                        m_nextTangent.X = m_nextKey.Time - Time;
                        m_nextTangent.Y = 0.0;
                    }
                    break;
                case SlopeMethod.SLOPE_SMOOTH:
                    bCalcSmoothNext = true;
                    break;
                case SlopeMethod.SLOPE_CLAMPED:
                    {
                        double fClampTolerence = (m_motionCurve.MaxValue - m_motionCurve.MinValue) * CLAMPTOLERANCE;
                        if (m_prevKey != null && Math.Abs(m_prevKey.Value - Value) <= fClampTolerence) // make Flat
                        {
                            if (m_nextKey != null)
                            {
                                m_nextTangent.X = m_nextKey.Time - Time;
                                m_nextTangent.Y = 0.0;
                            }
                            else
                            {
                                m_nextTangent.X = 0.0;
                                m_nextTangent.Y = 0.0;
                            }
                        }
                        else if (m_nextKey != null && Math.Abs(m_nextKey.Value - Value) <= fClampTolerence) // Make Flat
                        {
                            m_nextTangent.X = m_nextKey.Time - Time;
                            m_nextTangent.Y = 0.0;
                        }
                        else
                            bCalcSmoothNext = true;
                        break;
                    }
                case SlopeMethod.SLOPE_PLATEAU:
                    if (m_prevKey == null || m_nextKey == null)
                    {
                        if (m_nextKey != null)
                        {
                            m_nextTangent.X = m_nextKey.Time - Time;
                            m_nextTangent.Y = 0.0;
                        }
                        else
                        {
                            m_nextTangent.X = 0.0;
                            m_nextTangent.Y = 0.0;
                        }
                    }
                    else // we have a prev and a next, lets see if both the prev's out tangent and the next's in tangent are both either greater or less than our value, if so lets make out tangent flat
                    {
                        double fPrevTangentValue;
                        if (m_prevKey.m_nextSlopeMethod == SlopeMethod.SLOPE_PLATEAU)
                            fPrevTangentValue = m_prevKey.Value; // This way we don't get an infinite recursion
                        else
                        {
                            MeanVector2 vPrevPos = new MeanVector2(m_prevKey.Time, m_prevKey.Value);
                            MeanVector2 vPrevTangent = MeanVector2.Add(MeanVector2.Multiply(m_prevKey.NextTangent, 1.0 / 3.0), vPrevPos);
                            fPrevTangentValue = vPrevTangent.Y;
                        }

                        double fNextTangentValue;
                        if (m_nextKey.m_prevSlopeMethod == SlopeMethod.SLOPE_PLATEAU)
                            fNextTangentValue = m_nextKey.Value; // This way we don't get an infinite recursion
                        else
                        {
                            MeanVector2 vNextPos = new MeanVector2(m_nextKey.Time, m_nextKey.Value);
                            MeanVector2 vNextTangent = MeanVector2.Subtract(vNextPos, MeanVector2.Multiply(m_nextKey.PrevTangent, 1.0 / 3.0));
                            fNextTangentValue = vNextTangent.Y;
                        }

                        double fValue = Value;
                        if (fPrevTangentValue > fValue && fNextTangentValue > fValue)
                        {
                            m_nextTangent.X = m_nextKey.Time - Time;
                            m_nextTangent.Y = 0.0;
                        }
                        else if (fPrevTangentValue < fValue && fNextTangentValue < fValue)
                        {
                            m_nextTangent.X = m_nextKey.Time - Time;
                            m_nextTangent.Y = 0.0;
                        }
                        else
                            bCalcSmoothNext = true;
                    }
                    break;
                case SlopeMethod.SLOPE_STEPPED:
                case SlopeMethod.SLOPE_STEPPED_NEXT:
                    break; // nothing to do, no tangents
            }

            if (bCalcSmoothPrev || bCalcSmoothNext)
            {
                if (m_prevKey != null && m_nextKey != null)
                {
                    MeanVector2 delta = MeanVector2.Subtract(m_nextKey.TimeValue, m_prevKey.TimeValue);
                    double weight = Math.Abs(delta.X);
                    if (weight == 0) // if keys are on top of one another (no tangents)
                    {
                        if (bCalcSmoothPrev)
                        {
                            m_prevTangent.X = 0.0;
                            m_prevTangent.Y = 0.0;
                        }
                        if (bCalcSmoothNext)
                        {
                            m_nextTangent.X = 0.0;
                            m_nextTangent.Y = 0.0;
                        }
                    }
                    else
                    {
                        delta = MeanVector2.Divide(delta, weight);

                        if (bCalcSmoothPrev)
                        {
                            double prevWeight = Time - m_prevKey.Time ;
                            m_prevTangent = MeanVector2.Multiply(delta, prevWeight);
                        }
                        if (bCalcSmoothNext)
                        {
                            double nextWeight = m_nextKey.Time - Time;
                            m_nextTangent = MeanVector2.Multiply(delta, nextWeight);
                        }
                    }
                }
                else
                {
                    if (m_nextKey != null)
                    {
                        if (bCalcSmoothPrev)
                            m_prevTangent = MeanVector2.Subtract(m_nextKey.TimeValue, TimeValue);

                        if (bCalcSmoothNext)
                            m_nextTangent = MeanVector2.Subtract(m_nextKey.TimeValue, TimeValue);
                    }

                    if (m_prevKey != null)
                    {
                        if (bCalcSmoothPrev)
                            m_prevTangent = MeanVector2.Subtract(TimeValue, m_prevKey.TimeValue);

                        if (bCalcSmoothNext)
                            m_nextTangent = MeanVector2.Subtract(TimeValue, m_prevKey.TimeValue);
                    }
                }
            }

            m_prevTangent = MeanVector2.Multiply(m_prevTangent, PrevMagnitude);
            m_nextTangent = MeanVector2.Multiply(m_nextTangent, NextMagnitude);
        }

        #endregion
    }
}
