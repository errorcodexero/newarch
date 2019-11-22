#pragma once

#include "xeromath.h"
#include "CSVData.h"
#include <vector>


/// \file

namespace xero {
    namespace misc {

        /// \brief This class represents a single segmnet along a path.
        /// A path is made up of a series of closely space XeroAegment objects
        /// which represnt the position, velocity, acceleration, jerk, and heading
        /// for that location along the path.
        /// \sa XerOPath
        class XeroSegment {    
        public:
            /// \brief create a new segment with the given values
            /// \param x the x coordinate of the segment
            /// \param y the y coordinate of the segment
            /// \param linPos the linear position along the path for this segment
            /// \param vel the velocity along the path for this segment
            /// \param accel the acceleration along the path for this segment
            /// \param jerk the jerk along the path for this segment
            /// \param heading the heading along the path for this segment
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

            /// \brief create a new segment with the given values
            /// \param data the data for the segment, in order x, y, linpos, vel, accel, jerk, and heading
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

            /// \brief create a new segment with the given values
            /// \param data the data for the segment, in order x, y, linpos, vel, accel, jerk, and heading
            XeroSegment(std::vector<CSVData::CSVItem> data) {
                x_ = data[1].getDouble();
                y_ = data[2].getDouble();
                linPos_ = data[3].getDouble();
                vel_ = data[4].getDouble();
                accel_ = data[5].getDouble();
                jerk_ = data[6].getDouble();
                heading_ = data[7].getDouble();
            }

            /// \brief returns the X value of the segment
            /// \returns the X value of the segment
            double getX() const {
                return x_ ;
            }

            /// \brief returns the Y value of the segment
            /// \returns the Y value of the segment            
            double getY() const {
                return y_ ;
            }

            /// \brief returns the linear position along the segment
            /// \returns the linear position along the segment       
            double getPOS() const {
                return linPos_ ;
            }

            /// \brief returns the velocity along the segment
            /// \returns the velocity along the segment
            double getVelocity() const {
                return vel_ ;
            }

            /// \brief returns the acceleration along the segment
            /// \returns the acceleration along the segment
            double getAccel() const {
                return accel_ ;
            }

            /// \brief returns the jerk along the segment
            /// \returns the jerk along the segment
            double getJerk() const {
                return jerk_ ;
            }

            /// \brief returns the heading along the segment
            /// \returns the heading along the segment
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

