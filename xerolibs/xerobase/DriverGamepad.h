#pragma once

#include "HIDDevice.h"
#include "Action.h"
#include <frc/XBoxController.h>

/// \file

namespace xero {
    namespace base {
        class TankDrive ;

        /// \brief A DriveGamepad used to control the drivebase
        class DriverGamepad : public HIDDevice {
        public:
            /// \brief the axis number of the gamepad
            enum AxisNumber : int {
                LEFTX = 0,              ///< Left X axis
                LEFTY = 1,              ///< Left Y axis
                LTRIGGER = 2,           ///< Left Trigger Axis
                RTRIGGER = 3,           ///< Right Trigger Axis
                RIGHTX = 4,             ///< Right X axis
                RIGHTY = 5              ///< Right Y axis
            } ;

            /// \brief buttons on the gamepad
            enum ButtonNumber : int {
                A = 1,                  ///< A button
                B = 2,                  ///< B button
                X = 3,                  ///< X button
                Y = 4,                  ///< Y button
                LB = 5,                 ///< Left back button
                RB = 6,                 ///< Right back button
                BACK = 7,               ///< Back button
                START = 8,              ///< Start button
                L_JOY = 9,              ///< Left joystick button
                R_JOY = 10               ///< Right joystick button
            } ;

            /// \brief POV angles
            enum POVAngle : int {
                UP = 0,                 ///< Up, 0 degrees
                UPRIGHT = 45,           ///< UpRight, 45 degrees
                RIGHT = 90,             ///< Right, 90 degrees
                DOWNRIGHT = 135,        ///< DownRight, 135 degrees
                DOWN=180,               ///< Down, 180 degrees
                DOWNLEFT = 225,         ///< DownLeft, 225 degrees
                LEFT = 270,             ///< Left, 270 degrees
                UPLEFT = 315,           ///< UpLeft, 315 degrees
                NONE = -1,              ///< Not pressed in any direction
            } ;

        public:
            /// \brief create the driver gamepad object
            /// \param oi the OI subsystem for this gamepad
            /// \param index the index of the object
            DriverGamepad(OISubsystem &oi, int index) ;

            /// \brief destroy the object
            virtual ~DriverGamepad() ;

            /// \brief compute the state of the gamepad
            virtual void computeState() ;

            /// \brief genreation actions based on the HID device
            /// \param seq the action sequence for storing created actions
            virtual void generateActions(ActionSequence &seq) ;

            /// \brief associate the driver gamepad with a drive base
            /// \param db the drivebase to control
            void init(std::shared_ptr<TankDrive> db) ;

            void rumble(bool left, double value) ;

        private:
            double scalePower(double axis, double boost, bool slow) ;
            
        private:
            static const int SpinAxis = AxisNumber::RIGHTX ;
            static const int TurboAxis = AxisNumber::LTRIGGER ;

            static const int SlowButton = ButtonNumber::LB ;

        private:
            std::shared_ptr<TankDrive> db_ ;
            std::shared_ptr<frc::XboxController> controller_ ;

            int pov_ ;
            double default_duty_cycle_ ;
            double max_duty_cycle_ ;
            double slow_factor_ ;

            double left_ ;
            double right_ ; 
            double tolerance_ ;

            ActionPtr nudge_forward_ ;
            ActionPtr nudge_backward_ ;
            ActionPtr nudge_clockwise_ ;
            ActionPtr nudge_counter_clockwise_ ;
        } ;
     }
 }