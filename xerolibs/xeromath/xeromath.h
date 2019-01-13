#pragma once

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
        /// \param r the andle in radians
        /// \returns the angle in degrees
        inline double rad2deg(double r) {
            return r / PI * 180 ;
        }

        /// \brief convert an angle in degrees to radians
        /// \param d the andle in degrees
        /// \returns the angle in radians
        inline double deg2rad(double d) {
            return d / 180.0 * PI ;
        }
    }
}
