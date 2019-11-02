#pragma once

#include "xeromath.h"
#include "CSVData.h"
#include <vector>

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

            XeroSegment(std::vector<double> data) 
            {
                x_ = data[0] ;
                y_ = data[1]  ;
                linPos_ = data[2] ;
                vel_ = data[3] ;
                accel_ = data[4] ;
                jerk_ = data[5] ;
                heading_ = data[6] ;
            }

            XeroSegment(std::vector<CSVData::CSVItem> data) {
                x_ = data[0].getDouble();
                y_ = data[1].getDouble();
                linPos_ = data[2].getDouble();
                vel_ = data[3].getDouble();
                accel_ = data[4].getDouble();
                jerk_ = data[5].getDouble();
                heading_ = data[6].getDouble();
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

