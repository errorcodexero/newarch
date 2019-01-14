#pragma once

#include "Point.h"
#include "xeromath.h"

/// \file

namespace xero {
    namespace math {

        /// \brief This class represents a point using polar coordiantes
        /// \sa https://en.wikipedia.org/wiki/Polar_coordinate_system
        class Polar {
        public:
            /// \brief create a new polar point
            /// \param radius the distance from the origin to the point
            /// \param angle the angle in degrees between the reference zero angle and the point
            Polar(double radius, double angle) ;

            /// \brief create a new polar point from a cartesian point
            /// \param pt the point to conver to polar
            Polar(const Point &pt) ;

            /// \brief destroy the polar point
            ~Polar() ;

            /// \brief convert the polar point to cartesian
            /// \returns a cartesian point
            Point toCartesian() const ;

            /// \brief rotate the point a given angle
            /// \param angle the angle to rotate the point
            /// \returns the new point rotated the angle given
            Polar rotate(double angle) {
                return Polar(radius_, normalizeAngleDegrees(angle_ + angle)) ;
            }

            /// \brief return the angle for the point
            /// \returns the angle in degrees for the point
            double getAngle() const {
                return angle_ ;
            }

            /// \brief return the distance from the origin to the point
            /// \returns the distance from the origin to the point
            double getRadius() const {
                return radius_ ;
            }

        private:
            double radius_ ;
            double angle_ ;
        } ;
    }
}