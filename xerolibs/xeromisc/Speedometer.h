#pragma once

#include <xeromath.h>
#include <cmath>
#include <iostream>

/// \file

namespace xero {
    namespace misc {
        /// \brief this class calculates velocity and acceleration given a set of distance samples over time
        /// The values are computed by considering a fixed size set of samples.  This size is set when the
        /// Speedometer object is created.
        class Speedometer {
        public:
            /// \brief Create a new Speedometer object
            /// \param samples the number of samples to process to calculate values
            /// \param angle if true, measuring angular velocity, normalize to degrees between -180 and 180
            Speedometer(size_t samples, bool angle = false) {
                samples_ = samples ;
                angle = angle_ ;
                times_.resize(samples) ;
                distances_.resize(samples) ;
                velocities_.resize(samples) ;
                accel_ = 0.0 ;
            }

            /// \brief return the latest distance value
            /// \returns the latest distance value
            double getDistance() const {
                return distances_[samples_ - 1] ;
            }

            /// \brief return the latest velocity value
            /// \returns the latest velocity value
            double getVelocity() const {
                return velocities_[samples_ - 1] ;
            }

            /// \brief return the latest acceleration
            /// \returns the latest acceleration
            double getAcceleration() const {
                return accel_ ;
            }

            /// \brief return the oldest distance value
            /// \returns the oldest distance value
            double getOldestDistance() const {
                return distances_[0] ;
            }

            /// \brief return the oldest velocity value
            /// \returns the oldest velocity value
            double getOldestVelocity() const {
                return velocities_[0] ;
            }

            /// \brief update the Speedometer object
            /// \param dtime the delta time since the last call
            /// \param pos the position 
            void update(double dtime, double pos) {
                double vel ;

                if (dtime > 1e-6) {

                    times_.push_back(dtime) ;
                    times_.erase(times_.begin()) ;

                    double total = 0.0 ;
                    for(size_t i = 1 ; i < times_.size() ; i++)
                        total += times_[i] ;

                    distances_.push_back(pos) ;
                    distances_.erase(distances_.begin()) ;
                    if (angle_)
                        vel = xero::math::normalizeAngleDegrees(getDistance() - getOldestDistance()) / total ;
                    else
                        vel = (getDistance() - getOldestDistance()) / total ;

                    velocities_.push_back(vel) ;
                    velocities_.erase(velocities_.begin()) ;
                        
                    accel_ = (getVelocity() - getOldestVelocity()) / total ;
                }
            }

        private:
            std::vector<double> distances_ ;
            std::vector<double> velocities_ ;
            std::vector<double> times_ ;
            double accel_ ;
            bool angle_ ;
            size_t samples_ ;
        } ;
    }
}