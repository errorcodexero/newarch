#pragma once

#include <string>
#include <vector>


namespace xero {
    namespace misc {
        class TrapezoidalProfile {
        public:
            TrapezoidalProfile(double max_accel, double max_decel, double max_velocity) ;
            virtual ~TrapezoidalProfile() ;

            void update(double dist, double start_velocity, double end_velocity) ;

            double getSpeed(double t) const ;
            double getDistance(double t) const ;

            std::string toString() ;

            double getTimeAccel() const {
                return ta_ ;
            }

            double getTimeCruise() const {
                return tc_ ;
            }

            double getTimeDecel() const {
                return td_ ;
            }

			double getActualMaxVelocity() const {
				return isneg_ ? -actual_max_velocity_ : actual_max_velocity_ ;
			}

			double getTimeForDistance(double dist) const ;

			double getStartVelocity() const {
				return start_velocity_ ;
			}

			double getEndVelocity() const {
				return end_velocity_ ;
			}

        private:
            double intspeed(double t) const ;
			double pickRoot(const std::vector<double> &roots) const;

        private:
            bool isneg_ ;
            
            double max_accel_ ;
            double max_decel_ ;
            double max_velocity_ ;
            double distance_ ;

            double actual_max_velocity_ ;
            double start_velocity_ ;
            double end_velocity_ ;

            double ta_ ;
            double td_ ;
            double tc_ ;

            std::string type_ ;
        } ;
    }
}