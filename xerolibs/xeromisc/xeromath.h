#pragma once

#include <cassert>
#include <cmath>

/// \file

namespace xero {
    namespace math {
        /// \brief the value of PI
        static constexpr double PI=3.1415926535897931 ;
        
        /// \brief normalize an angle to the range -180 to + 180 degrees
        /// \param a the angle to normalize
        /// \returns the normalized angle
        inline double normalizeAngleDegrees(double a) {
            while (a <= -180.0)
                a += 360.0 ;

            while (a > 180.0)
                a -= 360.0 ;
                
            return a ;          
        }

        /// \brief normalize an angle to the range -PI to PI radians
        /// \param a the angle to normalize
        /// \returns the normalized angle
        inline double normalizeAngleRadians(double a) {
            if (a <= -PI)
                a += 2 * PI ;
            else if (a > PI)
                a -= 2 * PI ;
            return a ;
        }

        /// \brief convert an angle in radians to degrees
        /// \param r the angle in radians
        /// \returns the angle in degrees
        inline double rad2deg(double r) {
            return r / PI * 180 ;
        }

        /// \brief convert an angle in degrees to radians
        /// \param d the angle in degrees
        /// \returns the angle in radians
        inline double deg2rad(double d) {
            return d / 180.0 * PI ;
        }

        /// \brief check if measured value is within a +/- percentage of a ***non-zero*** target value
        /// \param target the reference target value
        /// \param measured the measured value being compared to the target
        /// \param percentage the margin as a perfentage (e.g., "5" for 5% allow variance in either direction vs. the target)
        /// \returns true if measured is almost equal to the target  
        inline bool equalWithinPercentMargin(double target, double measured, double percentage) {
            assert(target != 0);
            return (std::fabs(100.0 * (measured - target)/target) <= percentage);
        }
    }
}
