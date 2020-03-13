#pragma once

#include "xeromath.h"

/// \file

namespace xero {
    namespace misc {
        /// \brief this class tracks the movement of the robot based on encoder and IMU data
        class Kinematics {
        public:
            /// \brief create a Kinematics object
            /// The scrub factor is the ratio of a theoritical turn expected based on the left and
            /// right wheels moving a given distance, and the actual turn observed.  This is basically
            /// a factor between an ideal robot and a real robot
            /// \param width the width of the robot
            /// \param scrub the scrub factor for the robot when turning
            Kinematics(double width, double scrub) ;

            /// \brief destroy the kinematic object
            virtual ~Kinematics() ;

            /// \brief return the width of the robot
            /// \returns the scrub for the robot
            double getWidth() const {
                return width_ ;
            }

            /// \brief return the scrube for the robot
            /// \returns the scrub for the robot
            double getScrub() const {
                return scrub_ ;
            }

            /// \brief get the current X position of the robot
            /// \returns the X position of the robot
            double getX() const {
                return x_ ;
            }

            /// \brief get the current Y position of the robot
            /// \returns the Y position of the robot
            double getY() const {
                return y_ ;
            }

            /// \brief get the current angle of the robot
            /// \returns the angle of the robot
            double getAngle() const {
                return angle_ ;
            }

            /// \brief set the current location and angle of the robot
            /// \param x the x position for the robot
            /// \param y the y position for the robot
            /// \param angle the current angle for the robot
            void set(double x, double y, double angle) ;

            /// \brief Returns true if set has been called on this model.
            /// Intended for determining whether we have been calibrated
            /// to an absolute position on the field.
            /// \returns true if the positoin has been set
            bool hasBeenSet() { return set_; }

            /// \brief called to tell the object the robot has moved
            /// This form of the move call does not include IMU data and therefore angle change is
            /// calculated based on the difference in left and right wheel movement and the scrub
            /// factor for the robot.
            /// \param right the distance the right wheel moved
            /// \param left the distance the left wheel moved
            void move(double right, double left) ;

            /// \brief called to tell the object the robot has moved
            /// This form of the move call uses the angle from the IMU and is considerable more accurate
            /// \param right the distance the right wheel moved
            /// \param left the distance the left wheel moved
            /// \param angle the new absolute angle of the robot
            void move(double right, double left, double angle) ;

        private:
            static constexpr double StraightTolerance = 1e-6 ;

        private:
            double x_ ;
            double y_ ;
            double angle_ ;
            double scrub_ ;
            double width_ ;

            bool set_;
        } ;
    }
}
