using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace MeanMachineGenerator
{
    class MotionCurve
    {
        #region enums
        public enum ExtrapolationMethods
        {
            EXTRAPOLATION_CONSTANT,
            EXTRAPOLATION_LINEAR,
            EXTRAPOLATION_CYCLE,
            EXTRAPOLATION_CYCLE_RELATIVE,
            EXTRAPOLATION_OSCILLATE
        };
        #endregion

        #region private constants
        private const double EPSILON = 1E-8;
        private const double MAXFRAMEERROR = 0.003;
        #endregion

        #region private variables
        private MotionKey m_headKey;
        private MotionKey m_tailKey;
        private double m_defaultValue;
        private double m_minValue;
        private double m_maxValue;
        private double m_lastValue;
        private double m_lastDerivative;
        private double m_lastTime;
        private bool m_bLastTimeValid;
        private MotionKey m_lastAccessedKey;
        private ExtrapolationMethods m_preExtrapolation;
        private ExtrapolationMethods m_postExtrapolation;
        #endregion

        #region public constrcutors
        public MotionCurve()
        {
            m_headKey = null;
            m_tailKey = null;
            m_defaultValue = 0;
            m_minValue = Double.MinValue;
            m_maxValue = Double.MaxValue;
            m_lastValue = 0;
            m_lastDerivative = 0;
            m_lastTime = 0;
            m_bLastTimeValid = false;
            m_lastAccessedKey = null;
            m_preExtrapolation = ExtrapolationMethods.EXTRAPOLATION_CONSTANT;
            m_postExtrapolation = ExtrapolationMethods.EXTRAPOLATION_CONSTANT;
        }
        #endregion

        #region public properties
        public double MaxValue
        {
            get { return m_maxValue; }
            set { m_maxValue = value; }
        }

        public double MinValue
        {
            get { return m_minValue; }
            set { m_minValue = value; }
        }

        public MotionKey HeadKey
        {
            get { return m_headKey; }
            set { m_headKey = value; }
        }

        public MotionKey TailKey
        {
            get { return m_tailKey; }
            set { m_tailKey = value; }
        }

        public MotionKey LastAccessedKey
        {
            get { return m_lastAccessedKey; }
            set { m_lastAccessedKey = value; }
        }

        public double Length
        {
            get
            {
                if (m_tailKey == null)
                    return 0.0;

                return m_tailKey.Time;
            }
        }
        #endregion

        #region public methods

        public void OnKeyPositionChanged(MotionKey key)
        {
            m_bLastTimeValid = false;
        }

        public void RemoveAll()
        {
            MotionKey key = m_headKey;
            while (key != null)
            {
                MotionKey next = key.NextKey;
                key.PrevKey = null;
                key.NextKey = null;

                key = next;
            }

            m_headKey = null;
            m_tailKey = null;
        }

        public void RemoveKey(MotionKey key)
        {
            if (key.PrevKey == null)
                m_headKey = key.NextKey;
            else
            {
                key.PrevKey.NextKey = key.NextKey;
                key.PrevKey.OnPositionChanged();
            }

            if (key.NextKey == null)
                m_tailKey = key.PrevKey;
            else
            {
                key.NextKey.PrevKey = key.PrevKey;
                key.NextKey.OnPositionChanged();
            }
        }

       
        public void AddKey(MotionKey newKey)  // adds the key to the end
        {
            InsertKeyAfter(m_tailKey, newKey);
        }

        public MotionKey GetKey(double time)
        {
            MotionKey pKey;
            if (m_lastAccessedKey != null)
                pKey = m_lastAccessedKey;
            else
                pKey = m_headKey;

            if (pKey == null)
                return null;

            if (Math.Abs(pKey.Time - time) > EPSILON)
            {
                if (pKey.Time < time)
                {
                    for (pKey = pKey.NextKey; pKey != null && Math.Abs(pKey.Time - time) > EPSILON; pKey = pKey.NextKey)
                        ;
                }
                else
                {
                    for (pKey = pKey.PrevKey; pKey != null && Math.Abs(pKey.Time - time) > EPSILON; pKey = pKey.PrevKey)
                        ;
                }
            }

            if (pKey != null && m_lastAccessedKey != pKey)
            {
                m_lastAccessedKey = pKey;
                m_bLastTimeValid = false;
            }

            return pKey;
        }

        public MotionKey StoreValue(double time, double value)
        {
            MotionKey motionKey = createMotionKey(time);
            if (motionKey != null)
                motionKey.Value = value;
            return motionKey;
        }

        public MotionKey StoreValueSlopeAndMagnitude(double time, double value, double slope, double magnitude)
        {
            MotionKey motionKey = createMotionKey(time);
            if (motionKey != null)
            {
                motionKey.Value = value;
                MeanVector2 angleAndMagnitude = new MeanVector2(Math.Atan(slope), magnitude);
                motionKey.NextAngleAndMagnitude = angleAndMagnitude;
                motionKey.PrevAngleAndMagnitude = angleAndMagnitude;
            }
            motionKey.IsMarkBeginOrEndKeysToZeroSlope = false;
            return motionKey;
        }

        public double GetValue(double time)
        {
            if (HeadKey == null)
                return m_defaultValue;

            // post-extrapolation
            if (time > TailKey.Time)
            {
                switch (m_postExtrapolation)
                {
                    case ExtrapolationMethods.EXTRAPOLATION_CONSTANT:
                        return TailKey.Value;
                    case ExtrapolationMethods.EXTRAPOLATION_LINEAR:
                        {
                            MeanVector2 v2Slope = TailKey.PrevTangent;
                            return TailKey.Value + (v2Slope.Y / v2Slope.X) * (time - TailKey.Time);
                        }
                    case ExtrapolationMethods.EXTRAPOLATION_CYCLE:
                        {
                            double tStartdouble = HeadKey.Time;
                            double tLength = TailKey.Time - tStartdouble;
                            if (tLength != 0)
                            {
                                double tdoubleSinceStart = time - tStartdouble;
                                double tdoubleInto = tdoubleSinceStart % tLength;
                                double tNewdouble = tStartdouble - tdoubleInto;
                                return GetValue(tNewdouble);
                            }
                            else
                                return GetValue(tStartdouble);
                        }
                    case ExtrapolationMethods.EXTRAPOLATION_CYCLE_RELATIVE:
                        {
                            double tStartdouble = HeadKey.Time;
                            double tLength = TailKey.Time - tStartdouble;
                            if (tLength != 0)
                            {
                                double tdoubleSinceStart = time - tStartdouble;
                                double tdoubleInto = tdoubleSinceStart % tLength;
                                double tNewdouble = tStartdouble + tdoubleInto;

                                int nCount = (int)(tdoubleSinceStart / tLength);
                                double fHeight = TailKey.Value - HeadKey.Value;
                                return fHeight * nCount + GetValue(tNewdouble);
                            }
                            else
                                return GetValue(tStartdouble);
                        }
                    case ExtrapolationMethods.EXTRAPOLATION_OSCILLATE:
                        {
                            double tStartdouble = HeadKey.Time;
                            double tLength = TailKey.Time - tStartdouble;
                            if (tLength != 0)
                            {
                                double tdoubleSinceStart = time - tStartdouble;
                                double tdoubleInto = tdoubleSinceStart % tLength;
                                double tNewdouble = tStartdouble + tdoubleInto;

                                int nCount = (int)(tdoubleSinceStart / tLength);
                                if (nCount % 2 != 0)
                                    return GetValue(tLength - tNewdouble);
                                else
                                    return GetValue(tNewdouble);
                            }
                            else
                                return GetValue(tStartdouble);
                        }
                }
            }

            // pre-extrapolation
            if (time < HeadKey.Time)
            {
                switch (m_preExtrapolation)
                {
                    case ExtrapolationMethods.EXTRAPOLATION_CONSTANT:
                        return HeadKey.Value;
                    case ExtrapolationMethods.EXTRAPOLATION_LINEAR:
                        {
                            MeanVector2 v2Slope = HeadKey.NextTangent;
                            return HeadKey.Value + (v2Slope.Y / v2Slope.X) * (time - HeadKey.Time);
                        }
                    case ExtrapolationMethods.EXTRAPOLATION_CYCLE:
                        {
                            double tStartdouble = HeadKey.Time;
                            double tEnddouble = TailKey.Time;
                            double tLength = tEnddouble - tStartdouble;
                            if (tLength != 0)
                            {
                                double tdoubleSinceEnd = tEnddouble - time;
                                double tdoubleInto = tdoubleSinceEnd % tLength;
                                double tNewdouble = tEnddouble - tdoubleInto;
                                return GetValue(tNewdouble);
                            }
                            else
                                return GetValue(tStartdouble);
                        }
                    case ExtrapolationMethods.EXTRAPOLATION_CYCLE_RELATIVE:
                        {
                            double tStartdouble = HeadKey.Time;
                            double tEnddouble = TailKey.Time;
                            double tLength = tEnddouble - tStartdouble;
                            if (tLength != 0)
                            {
                                double tdoubleSinceEnd = tEnddouble - time;
                                double tdoubleInto = tdoubleSinceEnd % tLength;
                                double tNewdouble = tEnddouble - tdoubleInto;

                                int nCount = (int)(tdoubleSinceEnd / tLength);
                                double fHeight = HeadKey.Value - TailKey.Value;
                                return fHeight * nCount + GetValue(tNewdouble);
                            }
                            else
                                return GetValue(tStartdouble);
                        }
                    case ExtrapolationMethods.EXTRAPOLATION_OSCILLATE:
                        {
                            double tStartdouble = HeadKey.Time;
                            double tEnddouble = TailKey.Time;
                            double tLength = tEnddouble - tStartdouble;
                            if (tLength != 0)
                            {
                                double tdoubleSinceEnd = tEnddouble - time;
                                double tdoubleInto = tdoubleSinceEnd % tLength;
                                double tNewdouble = tEnddouble - tdoubleInto;

                                int nCount = (int)(tdoubleSinceEnd / tLength);
                                if (nCount % 2 != 0)
                                    return GetValue(tLength - tNewdouble);
                                else
                                    return GetValue(tNewdouble);
                            }
                            else
                                return GetValue(tStartdouble);
                        }
                }
            }

            if (m_lastAccessedKey != null)
            {
                if (m_bLastTimeValid && time == m_lastTime)
                    return m_lastValue; // if same as last time
            }
            else // if last key is not valid start from the beginning
            {
                m_lastAccessedKey = HeadKey;
            }

            if (m_lastAccessedKey.Time <= time)
            {
                for (MotionKey key = m_lastAccessedKey; key != null; key = key.NextKey)
                {
                    MotionKey nextKey = key.NextKey;
                    if (key.Time == time)
                    {
                        m_lastAccessedKey = key;
                        m_lastValue = key.Value;
                        break;
                    }
                    else if (nextKey.Time == time)
                    {
                        m_lastAccessedKey  = nextKey;
                        m_lastValue = nextKey.Value;
                        break;
                    }
                    else if (nextKey.Time > time)
                    {
                        m_lastAccessedKey = nextKey;
                        m_lastValue = InterpolateValue(time, key);
                        break;
                    }
                }
            }
            else
            {
                for (MotionKey key = m_lastAccessedKey.PrevKey; key != null; key = key.PrevKey)
                {
                    MotionKey nextKey = key.NextKey;
                    if (key.Time == time)
                    {
                        m_lastAccessedKey = key;
                        m_lastValue = key.Value;
                        break;
                    }
                    else if (nextKey.Time == time)
                    {
                        m_lastAccessedKey = nextKey;
                        m_lastValue = nextKey.Value;
                        break;
                    }
                    else if (key.Time < time)
                    {
                        m_lastAccessedKey = nextKey;
                        m_lastValue = InterpolateValue(time, key);
                        break;
                    }
                }
            }

            m_lastTime = time;
            m_bLastTimeValid = true;
            return m_lastValue;
        }

        public double GetDerivative(double time)
        {
            if (HeadKey == null || HeadKey == TailKey)
                return 0;

            if (m_lastAccessedKey.Time <= time)
            {
                for (MotionKey key = m_lastAccessedKey; key != null; key = key.NextKey)
                {
                    MotionKey nextKey = key.NextKey;
                    if (nextKey == null)
                        return m_lastDerivative;
                    if (key.Time == time)
                    {
                        MeanVector2 tangent = key.NextTangent;
                        m_lastDerivative = tangent.Y / tangent.X;
                        break;
                    }
                    else if (nextKey.Time == time)
                    {
                        MeanVector2 tangent = nextKey.PrevTangent;
                        m_lastDerivative = tangent.Y / tangent.X;
                        break;
                    }
                    else if (nextKey.Time > time)
                    {
                        m_lastDerivative = Derivative(time, key);
                        break;
                    }
                }
            }
            else
            {
                for (MotionKey key = m_lastAccessedKey.PrevKey; key != null; key = key.PrevKey)
                {
                    MotionKey nextKey = key.NextKey;
                    if (nextKey == null)
                        return m_lastDerivative;
                    if (key.Time == time)
                    {
                        MeanVector2 tangent = key.NextTangent;
                        m_lastDerivative = tangent.Y / tangent.X;
                        break;
                    }
                    else if (nextKey.Time == time)
                    {
                        MeanVector2 tangent = nextKey.PrevTangent;
                        m_lastDerivative = tangent.Y / tangent.X;
                        break;
                    }
                    else if (key.Time < time)
                    {
                        m_lastDerivative = Derivative(time, key);
                        break;
                    }
                }
            }

            return m_lastDerivative;
        }

        public void fixUpTailAndPrevPointers()
        {
            MotionKey prevKey = null;
            for (MotionKey key = m_headKey; key != null; key = key.NextKey)
            {
                key.PrevKey = prevKey;
                key.Curve = this;
                prevKey = key;
            }
            m_tailKey = prevKey;
        }
        #endregion

        #region private methods
        private void InsertKeyBefore(MotionKey atKey, MotionKey newKey)
        {
            newKey.Curve = this;

            if (atKey == m_headKey)
            {
                m_headKey = newKey;
                if (m_tailKey == null)
                    m_tailKey = m_headKey;
            }

            m_lastAccessedKey = newKey;
            m_bLastTimeValid = false;

            if (atKey != null)
            {
                if (newKey.Time > atKey.Time)
                {
                    // System.out.println("Out of order keys detected.");
                }
                newKey.InsertBefore(atKey);
            }

            newKey.OnPositionChanged();
        }

        private void InsertKeyAfter(MotionKey atKey, MotionKey newKey)
        {
            newKey.Curve = this;

            if (atKey == m_tailKey)
            {
                m_tailKey = newKey;
                if (m_headKey == null)
                    m_headKey = m_tailKey;
            }

            m_lastAccessedKey = newKey;
            m_bLastTimeValid = false;

            if (atKey != null)
            {
                Debug.Assert(newKey.Time >= atKey.Time);
                newKey.InsertAfter(atKey);
            }

            newKey.OnPositionChanged();
        }


        private double Derivative(double time, MotionKey pKey)
        {
            MotionKey pNextKey = pKey.NextKey;

            MotionKey.SlopeMethod nextSlopeMethod = pKey.NextSlopeMethod;
            MotionKey.SlopeMethod prevSlopeMethod = pNextKey.PrevSlopeMethod;

            if (nextSlopeMethod == MotionKey.SlopeMethod.SLOPE_STEPPED)
                return 0;
            else if (nextSlopeMethod == MotionKey.SlopeMethod.SLOPE_STEPPED_NEXT)
                return 0;
            else if (nextSlopeMethod == MotionKey.SlopeMethod.SLOPE_LINEAR && prevSlopeMethod == MotionKey.SlopeMethod.SLOPE_LINEAR)
            {
                return (pNextKey.Value - pKey.Value) / (pNextKey.Time - pKey.Time);
            }
            else
            {
                double evalx = time;
                double pointax = pKey.Time;
                double pointbx = pNextKey.Time;
                double xspan = pointbx - pointax;
                double guesst = (evalx - pointax) / xspan;

                // if the weights are default, then the x cubic is linear and there is no need to evaluate it
                if (pKey.NextMagnitude == 1.0f && pNextKey.PrevMagnitude == 1.0f)
                    return pKey.YCoefficients.FirstDerivative(guesst);

                // Spline - non default tangents means that we need a second parametric cubic for x as a function of t
                double diffx = evalx - pKey.XCoefficients.Evaluate(guesst);
                double error = Math.Abs(diffx);
                double maxerror = MAXFRAMEERROR / 30.0f;

                if (error > maxerror)
                {
                    double positiveError = Double.MaxValue;
                    double negativeError = Double.MinValue;

                    if (diffx > 0)
                        positiveError = diffx;
                    else
                        negativeError = diffx;

                    while (error > maxerror)
                    {
                        guesst = guesst + diffx / pKey.XCoefficients.FirstDerivative(guesst);
                        diffx = evalx - pKey.XCoefficients.Evaluate(guesst);
                        error = Math.Abs(diffx);

                        if ((diffx > 0 && diffx > positiveError) || (diffx < 0 && diffx < negativeError))
                        {
                            // NOT CONVERGING, PROBABLY BOGUS CHANNEL DATA, WALK USING BUMP FD
                            Debug.Assert(false);
                            maxerror = 1.0f / 100.0f;  // DON'T BE AS ACCURATE BECAUSE THIS IS MUCH SLOWER
                            int steps = (int)(xspan / maxerror);
                            steps = Math.Min(steps, 1000);
                            double deltat = 1.0f / steps;
                            pKey.XCoefficients.InitFD(steps);
                            int i;
                            diffx = error;
                            for (i = 0, guesst = 0.0; diffx > maxerror && i < steps; guesst += deltat, i++)
                                diffx = Math.Abs(evalx - pKey.XCoefficients.BumpFD());
                            break;
                        }

                        if (diffx > 0)
                            positiveError = diffx;
                        else
                            negativeError = diffx;
                    }
                }

                return pKey.YCoefficients.FirstDerivative(guesst);
            }
        }

        private double InterpolateValue(double time, MotionKey pKey)
        {
            MotionKey pNextKey = pKey.NextKey;

            MotionKey.SlopeMethod nextSlopeMethod = pKey.NextSlopeMethod;
            MotionKey.SlopeMethod prevSlopeMethod = pNextKey.PrevSlopeMethod;

            if (nextSlopeMethod == MotionKey.SlopeMethod.SLOPE_STEPPED)
                return pKey.Value;
            else if (nextSlopeMethod == MotionKey.SlopeMethod.SLOPE_STEPPED_NEXT)
                return pNextKey.Value;
            else if (nextSlopeMethod == MotionKey.SlopeMethod.SLOPE_LINEAR && prevSlopeMethod == MotionKey.SlopeMethod.SLOPE_LINEAR)
            {
                return pKey.Value + (time - pKey.Time)
                        / (pNextKey.Time - pKey.Time)
                        * (pNextKey.Value - pKey.Value);
            }
            else
            {
                double evalx = time;
                double pointax = pKey.Time;
                double pointbx = pNextKey.Time;
                double xspan = pointbx - pointax;
                double guesst = (evalx - pointax) / xspan;

                // if the weights are default, then the x cubic is linear and there is no need to evaluate it
                if (Math.Abs(pKey.NextMagnitude - 1.0f) < EPSILON && Math.Abs(pNextKey.PrevMagnitude - 1.0f) < EPSILON)
                    return pKey.YCoefficients.Evaluate(guesst);

                // Spline - non default tangents means that we need a second parametric cubic for x as a function of t
                double diffx = evalx - pKey.XCoefficients.Evaluate(guesst);
                double error = Math.Abs(diffx);
                double maxerror = MAXFRAMEERROR / 30.0f;

                if (error > maxerror)
                {
                    double positiveError = Double.MaxValue;
                    double negativeError = Double.MinValue;

                    if (diffx > 0)
                        positiveError = diffx;
                    else
                        negativeError = diffx;

                    while (error > maxerror)
                    {
                        guesst = guesst + diffx / pKey.XCoefficients.FirstDerivative(guesst);
                        diffx = evalx - pKey.XCoefficients.Evaluate(guesst);
                        error = Math.Abs(diffx);

                        if ((diffx > 0 && diffx > positiveError) || (diffx < 0 && diffx < negativeError))
                        {
                            // NOT CONVERGING, PROBABLY BOGUS CHANNEL DATA, WALK USING BUMP FD
                            Debug.Assert(false);
                            maxerror = 1.0f / 100.0f;  // DON'T BE AS ACCURATE BECAUSE THIS IS MUCH SLOWER
                            int steps = (int)(xspan / maxerror);
                            steps = Math.Min(steps, 1000);
                            double deltat = 1.0f / steps;
                            pKey.XCoefficients.InitFD(steps);
                            int i;
                            diffx = error;
                            for (i = 0, guesst = 0.0; diffx > maxerror && i < steps; guesst += deltat, i++)
                                diffx = Math.Abs(evalx - pKey.XCoefficients.BumpFD());
                            break;
                        }

                        if (diffx > 0)
                            positiveError = diffx;
                        else
                            negativeError = diffx;
                    }
                }

                return pKey.YCoefficients.Evaluate(guesst);
            }
        }

        private MotionKey findClosestKey(double time)
        {
            MotionKey pKey;
            if (m_lastAccessedKey != null)
                pKey = m_lastAccessedKey;
            else
                pKey = m_headKey;

            if (pKey == null)
                return null;

            if (pKey.Time != time)
            {
                if (pKey.Time < time)
                {
                    MotionKey pNextKey = pKey.NextKey;
                    while (pNextKey != null)
                    {
                        if (pNextKey.Time > time)
                            break;
                        pKey = pNextKey;
                        pNextKey = pKey.NextKey;
                    }
                }
                else
                {
                    MotionKey pPrevKey = pKey.PrevKey;
                    while (pPrevKey != null)
                    {
                        pKey = pPrevKey;
                        if (pKey.Time <= time)
                            break;
                        pPrevKey = pKey.PrevKey;
                    }
                }
            }

            m_lastAccessedKey = pKey;
            m_bLastTimeValid = false;

            return pKey;
        }

        private MotionKey createMotionKey(double time)
        {
            MotionKey pKey = findClosestKey(time);
            MotionKey pNewKey;

            if (pKey != null && pKey.Time == time)
                pNewKey = pKey;
            else
            {
                pNewKey = new MotionKey();
                pNewKey.Curve = this;
                pNewKey.Time = time;

                if (pKey == null)
                    AddKey(pNewKey);
                else if (pKey.Time <= time)
                    InsertKeyAfter(pKey, pNewKey);
                else if (pKey.Time > time)
                    InsertKeyBefore(pKey, pNewKey);
            }

            // for motion profiling, we want the first and last keys to be 0 slope, but all others to be normally smooth
            if (pNewKey == m_headKey && pNewKey.IsMarkBeginOrEndKeysToZeroSlope)
            {
                pNewKey.PrevSlopeMethod = MotionKey.SlopeMethod.SLOPE_FLAT;
                pNewKey.NextSlopeMethod = MotionKey.SlopeMethod.SLOPE_FLAT;
                if (pNewKey.NextKey != null && pNewKey.NextKey != m_tailKey)
                {  // the former head is not also the tail
                    pNewKey.NextKey.NextSlopeMethod = MotionKey.SlopeMethod.SLOPE_SMOOTH;
                    pNewKey.NextKey.PrevSlopeMethod = MotionKey.SlopeMethod.SLOPE_SMOOTH;
                }
            }
            else if (pNewKey == m_tailKey && pNewKey.IsMarkBeginOrEndKeysToZeroSlope)
            {
                pNewKey.PrevSlopeMethod = MotionKey.SlopeMethod.SLOPE_FLAT;
                pNewKey.NextSlopeMethod = MotionKey.SlopeMethod.SLOPE_FLAT;
                if (pNewKey.PrevKey != null && pNewKey.PrevKey != m_headKey)
                {  // the former tail is not also the head
                    pNewKey.PrevKey.NextSlopeMethod = MotionKey.SlopeMethod.SLOPE_SMOOTH;
                    pNewKey.PrevKey.PrevSlopeMethod = MotionKey.SlopeMethod.SLOPE_SMOOTH;
                }
            }
            else
            {
                pNewKey.PrevSlopeMethod = MotionKey.SlopeMethod.SLOPE_SMOOTH;
                pNewKey.NextSlopeMethod = MotionKey.SlopeMethod.SLOPE_SMOOTH;
            }

            m_lastAccessedKey = pNewKey;
            m_bLastTimeValid = false;

            return pNewKey;
        }

        #endregion
    }
}
