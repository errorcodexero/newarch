#include "TrapezoidalProfile.h"
#include <cmath>
#include <iostream>

namespace xero {
    namespace misc {
		TrapezoidalProfile::TrapezoidalProfile() {
			init(0.0, 0.0, 0.0);
		}

        TrapezoidalProfile::TrapezoidalProfile(double max_accel, double max_decel, double max_velocity) {
			init(max_accel, max_decel, max_velocity);
        }

        TrapezoidalProfile::~TrapezoidalProfile() {
        }

		void TrapezoidalProfile::init(double max_accel, double max_decel, double max_velocity) {
			max_accel_ = max_accel;
			max_decel_ = max_decel;
			max_velocity_ = max_velocity;
            isneg_ = false ;
		}

		void TrapezoidalProfile::initFromSettings(SettingsParser &settings, const std::string &prefix) {
			double max_accel = settings.getDouble(prefix + ":max_a");
			double max_decel = settings.getDouble(prefix + ":max_d");
			double max_velocity = settings.getDouble(prefix + ":max_v");
			init(max_accel, max_decel, max_velocity);
		}
		
        double TrapezoidalProfile::intspeed(double t) {
            double ret ;
            if (t < 0.0) {
                ret = start_velocity_ ;
            }
            else if (t < ta_) {
                ret = start_velocity_ + t * max_accel_ ;
            }   
            else if (t < ta_ + tc_) {
                ret = actual_max_velocity_ ;
            }   
            else if (t < ta_ + tc_ + td_) {
				double dt = (t - ta_ - tc_) ;
                ret = actual_max_velocity_ + dt * max_decel_ ;
            }
            else {
                ret = end_velocity_ ;
            }

            return ret ;
        }

        double TrapezoidalProfile::getSpeed(double t) {
            double ret = intspeed(t) ;
            return isneg_ ? -ret : ret ;
        }

        double TrapezoidalProfile::getDistance(double t) {
            double ret ;

            if (t > 0.75)
                ret = 0.0 ;

            if (t < 0.0) {
                ret = 0.0 ;
            }
            else if (t < ta_) {
                ret = start_velocity_ * t + 0.5 * t * t * max_accel_ ;
            }   
            else if (t < ta_ + tc_) {
                ret = start_velocity_ * ta_ + 0.5 * ta_ * ta_ * max_accel_ ;
                ret += (t - ta_) * actual_max_velocity_ ;
            }   
            else if (t < ta_ + tc_ + td_) {
                double dt = t - ta_ - tc_ ;
                ret = start_velocity_ * ta_ + 0.5 * ta_ * ta_ * max_accel_ ;
                ret += tc_ * actual_max_velocity_ ;
                ret += actual_max_velocity_ * dt + 0.5 * dt * dt * max_decel_ ;
            }
            else {
                ret = distance_ ;
            }

            return isneg_ ? -ret : ret ;
        }        

        void TrapezoidalProfile::update(double dist, double start_velocity, double end_velocity) {
            start_velocity_ = std::fabs(start_velocity) ;
            end_velocity_ = std::fabs(end_velocity) ;

            isneg_ = (dist < 0) ;
            distance_ = std::fabs(dist) ;
            ta_ = (max_velocity_ - start_velocity_) / max_accel_ ;
            td_ = (end_velocity_ - max_velocity_) / max_decel_ ;
            double da = start_velocity * ta_ + 0.5 * max_accel_ * ta_ * ta_ ;
            double dd = max_velocity_ * td_ + 0.5 * max_decel_ * td_ * td_ ;
			tc_ = (distance_ - da - dd) / max_velocity_ ;
            type_ = "trapezoid" ;

            if (td_ < 0.0 || da + dd > distance_) {
                //
                // We don't have time to get to the cruising velocity
                //
                double num = (2.0 * distance_ * max_accel_ * max_decel_ + max_decel_ * start_velocity_ * start_velocity_ - max_accel_ * end_velocity_ * end_velocity_) / (max_decel_ - max_accel_) ;
				bool decel_only = false ;
				if (num < 0)
					decel_only = true ;
				else
					actual_max_velocity_ = std::sqrt(num) ;


				if (decel_only || actual_max_velocity_ < start_velocity_) {
                    // 
                    // Just decelerate down to the end
                    //
                    ta_ = 0 ;
                    tc_ = 0 ;
                    td_ = (end_velocity - start_velocity_) / max_decel_ ;
                    actual_max_velocity_ = start_velocity_ ;
                    type_ = "line" ;
                }
                else {
                    //
                    // Can't get to max velocity but can accelerate some
                    // before decelerating
                    //
                    actual_max_velocity_ = std::sqrt(num) ;
                    ta_ = (actual_max_velocity_ -start_velocity_)/ max_accel_ ;
                    td_ = (end_velocity_ -actual_max_velocity_) / max_decel_ ;
                    tc_ = 0 ;
                    type_ = "pyramid" ;
                }
            }
            else {
                //
                // Ok, now figure out the crusing time
                //
                actual_max_velocity_ = max_velocity_ ;                
                tc_ = (distance_ - da - dd) / max_velocity_ ;
            }
        }

        std::string TrapezoidalProfile::toString() {
            std::string ret = "[" + type_ ;
			ret += ", sv " + std::to_string(start_velocity_) ;
			ret += ", mv " + std::to_string(actual_max_velocity_) ;
			ret += ", ev " + std::to_string(end_velocity_) ;
            ret += ", ta " + std::to_string(ta_) ;
            ret += ", tc " + std::to_string(tc_) ;
            ret += ", td " + std::to_string(td_) ;                        
            ret += "]" ;
            return ret ;
        }
    }
}
