#pragma once
#include "SettingsParser.h"

/// \file

namespace xero {
    namespace misc{

        /// \brief a modified drivebase follower used to follow motion profiles
        /// This follower is intended to follow a motion profile that provides the
        /// desired position, velocity and acceleration.
        class PIDACtrl{
        public:

            /// \brief Create the follower with constants from the settings file
            /// \param parser the settings parser
            /// \param kv the name of the kv constant in the settings file
            /// \param ka the name of the ka constant in the settings file
            /// \param kp the name of the kp constant in the settings file
            /// \param kd the name of the kd constant in the settings file
            PIDACtrl(SettingsParser &parser, const std::string &kv, const std::string &ka, const std::string &kp, const std::string &kd, bool angle = false);

            /// \brief Create the follower with constants provided
            /// \param kv the kv constant
            /// \param ka the ka constant
            /// \param kp the kp constant
            /// \param kd the kd constant
            PIDACtrl(double kv, double ka, double kp, double kd, bool angle = false);

            /// \brief returns the output given the follower values
            /// \param a the desired acceleration
            /// \param v the desired velocity
            /// \param dtarget the target distance
            /// \param dactual the actual distance
            /// \param dt the delta time since the last time this was called
            double getOutput(double a, double v, double dtarget, double dactual, double dt);

        private:
            double kv_;
            double ka_;
            double kp_;
            double kd_;
            double last_error_;
            double angle_ ;
        };
    }
}