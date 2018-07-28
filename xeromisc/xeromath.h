#pragma once

namespace xero {
    namespace misc {
        static constexpr double PI= 3.14159265359 ;

        inline double angleDifference(double a1, double a2) {
            double diff = a1 - a2 ;
            if (diff <= -180)
                diff += 360 ;
            else if (diff > 180)
                diff -= 360 ;

            return diff ;
        }

        inline double rad2deg(double r) {
            return r / PI * 180 ;
        }

        inline double deg2rad(double d) {
            return d / 180.0 * PI ;
        }
    }
}
