#pragma once

namespace xero {
    namespace misc {
        class XeroPathSegment
        {
        public:
            XeroPathSegment(double x, double y, double pos, double vel, double acc, double jerk, double heading) {
                x_ = x ;
                y_ = y ;
                pos_ = pos ;
                vel_ = vel ;
                acc_ = acc ;
                jerk_ = jerk ;
                heading_ = heading ;
            }

            double getX() const { return x_ ;}
            double getY() const { return y_ ;}
            double getPOS() const { return pos_ ;}
            double getVelocity() const { return vel_ ;}
            double getAcceleration() const {return acc_ ;}
            double getJerk() const {return jerk_ ;}
            double getHeading() const { return heading_ ;}

        private:
            double x_ ;
            double y_ ;
            double pos_ ;
            double vel_ ;
            double acc_ ;
            double jerk_ ;
            double heading_ ;
        } ;
    }
}