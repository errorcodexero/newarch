#pragma once

#include "XeroPathConsts.h"
#include "xeromath.h"
#include <array>

namespace xero {
    namespace misc {

        class XeroSegment {    
        public:
            XeroSegment(double x, double y, double linPos,
                        double vel, double accel, double jerk, double heading) 
            {
                x_ = x ;
                y_ = y ;
                linPos_ = linPos ;
                vel_ = vel ;
                accel_ = accel ;
                jerk_ = jerk ;
                heading_ = heading ;
            }

            XeroSegment(std::array<double, HEADER_COUNT> data) 
            {
                x_ = data[0] ;
                y_ = data[1]  ;
                linPos_ = data[2] ;
                vel_ = data[3] ;
                accel_ = data[4] ;
                jerk_ = data[5] ;
                heading_ = data[6] ;
            }

            double getX() const {
                return x_ ;
            }
            double getY() const {
                return y_ ;
            }
            double getPOS() const {
                return linPos_ ;
            }
            double getVelocity() const {
                return vel_ ;
            }
            double getAccel() const {
                return accel_ ;
            }
            double getJerk() const {
                return jerk_ ;
            }
            double getHeading() const {
                return heading_ ;
            }                                                                        

        private:
            double x_;
            double y_;
            double linPos_;
            double vel_;
            double accel_;
            double jerk_;
            double heading_;
        };
    }
}

