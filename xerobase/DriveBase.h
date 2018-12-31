#pragma once

#include <Subsystem.h>
#include <Robot.h>

/// \file

namespace xero {
    namespace base {
        /// \brief the base class for all drivebase types
        class DriveBase : public Subsystem {
        public:
            /// \brief create a new drive base
            /// \param robot the robot the drivebase belongs to
            /// \param name the name of the drivebase subsystem
            DriveBase(Robot &robot, const std::string &name) : Subsystem(robot, name) {
            }

            /// \brief destroy the drive base
            virtual ~DriveBase() {
            }

            /// \brief returns the distance the drivebase has traveled
            /// \returns the distance the drivebase has traveled
            virtual double getDist() const = 0 ;

            /// \brief returns the angle of the drivebase
            /// \returns the angle of the drivebase
            virtual double getAngle() const = 0 ;

            /// \brief returns the velocity of the drivebase
            /// \returns the velocity of the drivebase
            virtual double getVelocity() const = 0 ;
        } ;
    }
}