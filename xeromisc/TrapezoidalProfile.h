#pragma once

#include <string>

namespace xero {
    namespace misc {
		/// \brief A trapazoidal velocity profile.
		///
		/// Creates a trapazoid-shaped velocity profile to drive the robot a given distance.
        /// Provides the target velocity on the profile from the current time.
        class TrapezoidalProfile {
        public:
			/// \brief Create a new trapazoidal velocity profile.
			/// \param max_accel the maximum acceleration in inches / second / second
			/// \param max_decel the maximum deceleration in inches / second / second
			/// \param max_velocity the maximum velocity in inches / second
            TrapezoidalProfile(double max_accel, double max_decel, double max_velocity) ;

			/// \brief Destroy the velocity profile.
            virtual ~TrapezoidalProfile() ;

			/// \brief Update the profile based on the current distance travelled and velocity
			/// and target end velocity.
			/// \param dist the distance travelled so far
			/// \param start_velocity the current velocity in inches / second
			/// \param end_velocity the target end velocity in inches / second
            void update(double dist, double start_velocity, double end_velocity) ;

			/// \brief Get the target speed based on the passed time.
			/// \param t the passed time in seconds
			/// \returns the target speed in inches / second
            double getSpeed(double t) ;

			/// \brief Get the distance that the robot should have travelled based on the profile.
			/// \param t the passed time in seconds
			/// \returns the distance that the robot should have travelled in inches
            double getDistance(double t) ;

			/// \brief Convert the profile to a string.
			/// \returns information about the profile
            std::string toString() ;

			/// \brief Get the profile's acceleration time
			/// \returns the profile's acceleration time in seconds
            double getTimeAccel() const {
                return ta_ ;
            }

			/// \brief Get the profile's cruising time
			/// \returns the profile's cruising time in seconds
            double getTimeCruise() const {
                return tc_ ;
            }

			/// \brief Get the profile's deceleration time
			/// \returns the profile's deceleration time in seconds
            double getTimeDecel() const {
                return td_ ;
            }

			/// \brief Get the actual maximum velocity accounting for the shape of the profile
			/// \return the actual maximum velocity in inches / second
			double getActualMaxVelocity() const {
				return isneg_ ? -actual_max_velocity_ : actual_max_velocity_ ;
			}

        private:
            double intspeed(double t) ;

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
