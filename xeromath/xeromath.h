#pragma once

namespace xero {
    namespace math {
        static constexpr double PI=3.1415926535897931 ;
		
		inline double normalizeAngleDegrees(double a) {
            if (a <= -180)
                a += 360 ;
            else if (a > 180)
                a -= 360 ;
            return a ;			
		}

		inline double normalizeAngleRadians(double a) {
            if (a <= -PI)
                a += 2 * PI ;
            else if (a > PI)
                a -= 2 * PI ;
            return a ;
		}

        inline double rad2deg(double r) {
            return r / PI * 180 ;
        }

        inline double deg2rad(double d) {
            return d / 180.0 * PI ;
        }
    }
}
