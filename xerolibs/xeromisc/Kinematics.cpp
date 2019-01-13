#include "Kinematics.h"
#include "xeromath.h"
#include <cmath>
#include <iostream>

using namespace xero::math ;

namespace xero {
    namespace misc {
        Kinematics::Kinematics(double width, double scrub) {
            x_ = 0.0 ;
            y_ = 0.0 ;
            angle_ = 0.0 ;

            width_ = width ;
            scrub_ = scrub ;
        }

        Kinematics::~Kinematics() {             
        }

        void Kinematics::set(double x, double y, double angle) {
            x_ = x ;
            y_ = y ;
            angle_ = angle ;
        }

        void Kinematics::move(double right, double left) {
            double dv = (right - left) / 2 * scrub_ ;
            double angle = angle_ + (dv * 2.0) / width_ ;

            move(right, left, angle) ;
        }

        void Kinematics::move(double right, double left, double angle) {
            angle_ = angle ;
            if (std::fabs(left - right) < StraightTolerance) {
                x_ += right * std::cos(angle_) ;
                y_ += right * std::sin(angle_) ;
            }
            else {
                double r = width_ * (left + right) / (2 * (right - left)) ;
                double wd = (right - left) / width_ ;
                x_ = x_ + r * std::sin(wd + angle_) - r * std::sin(angle_) ;
                y_ = y_ - r * std::cos(wd + angle_) + r * std::cos(angle_) ;
            }
        }
    }
}