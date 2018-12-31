#pragma once

#include "Point.h"

/// \file

namespace xero {
    namespace math {
        /// brief this class represents a position with a rotation at that position
        class PointAngle : public Point {
        public:
            /// \brief create a PositionAngle at the origin with angle zero
            PointAngle() ;

            /// \brief create a PositionAngle at the given x and y position and zero angle
            /// \param x the x position
            /// \param y the y position
            PointAngle(double x, double y);

            /// \brief create a PositionAngle at the given x and y position and the angle given
            /// \param x the x position
            /// \param y the y position
            /// \param angle the angle for the position
            PointAngle(double x, double y, double angle) ;

            /// \brief create a PositionAngle at the given point and angle
            /// \param pt the point for the object
            /// \param angle the angle for the object
            PointAngle(const Point &pt, double angle) ;

            /// \brief destroy the PositionAngle object
            ~PointAngle() ;

            /// \brief returns true if the angle is vertical (along the y axis)
            /// \returns true if the angle is vertical
            bool isVertical() const ;

            /// \brief returns true if the angle is horinzontal (along the x axis)
            /// \returns true if the angle is horizontal
            bool isHorizontal() const ;

            /// \brief returns the angle for the object
            /// \returns the angle for the object
            double getAngle() const {
                return angle_ ;
            }

            /// \brief returns a new PositionAngle that is a result of moving forward the given distance
            /// \param dist the distance to move
            /// \returns new PositionAngle after moving forward the given distance
            PointAngle forward(double dist) const ;

            /// \brief returns a new PositionAngle after translating it by the x and y offset given
            /// \param x the x offset for the translation
            /// \param y the y offset for the translation
            /// \returns the new PositionAngle after translation
            PointAngle translate(double x, double y) const ;

            /// \brief returns a new PositionAngle after translating it by the offset given
            /// \param offset the offset for the translation
            /// \returns the new PositionAngle after translation            
            PointAngle translate(const Point &offset) const ;

            /// \brief returns a new PositionAngle after rotating the existing one about the origin the angle given
            /// \param angle the angle of rotation
            /// \returns the new PositionAngle object after rotation
            PointAngle rotate(double angle) const ;

            /// \brief returns a new PositionAngle that is perpendicular to this one
            /// \returns a PositionAngle perpendicular to this one
            PointAngle perpendicular() const ;

            /// \brief returns a point that is the intersection of two lines
            /// If the two lines do not intersect, the point will contain NaN for both the
            /// X and Y coordinates
            /// \param p1 the first line
            /// \param p2 the second line
            /// \returns point of intersection between the two lines given by the two PointAngle objects
            static Point intersect(const PointAngle &p1, const PointAngle &p2) ;

            /// \brief return a human readable string of this object
            /// \returns a human readable string for this object
            std::string toString() const ;

        private:
            double static constexpr AngleTolerance = 1e-8 ;
            double static constexpr SlopeEqualTolerance = 1e-6 ;

        private:
            //
            // The angle, always in radians
            //
            double angle_ ;
        } ;
    }
}