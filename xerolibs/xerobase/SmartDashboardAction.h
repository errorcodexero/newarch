#pragma once
#include <frc/Timer.h>
#include <memory>
#include <vector>
#include <string>
#include <memory>

/// \file


namespace xero {
    namespace base {
        /// \brief A class capable of doing work.
        /// An action is the basis of all activity on the robot both in operator mode and autonomous mode.  
        /// There are some actions that are independent of subsystems are part of the control of the robot.  However
        /// the vast majority of actions are used to ask a robot subsystem to perform some function.  For instance,
        /// one of the actions for the TankDrive subsystem is to drive straight a fixed distance.
        /// \sa ParallelAction
        /// \sa DelayAction
        /// \sa SequenceAction
        class SmartDashboardAction {
        public:
            enum class DataType
            {
                Number,
                String, 
                Boolean
            } ;

        public:
            SmartDashboardAction(const std::string &name, double number) {
                name_ = name ;
                dt_ = DataType::Number ;
                number_ = number ;
            }

            SmartDashboardAction(const std::string &name, int number) {
                name_ = name ;                
                dt_ = DataType::Number ;
                number_ = number ;
            }

            SmartDashboardAction(const std::string &name, const std::string &str) {
                name_ = name ;                
                dt_ = DataType::String ;
                str_ = str ;
            }

            SmartDashboardAction(const std::string &name, bool b) {
                name_ = name ;                
                dt_ = DataType::Boolean ;
                boolvalue_ = b ;
            }                                    

            /// \brief Start the action; called once per action when it starts
            virtual void start()  ;

            /// \brief Manage the action; called each time through the robot loop
            virtual void run() ;

            /// \brief Cancel the action
            virtual void cancel() ;

            /// \brief Return true if the action is complete
            /// \returns True if the action is complete
            virtual bool isDone()  ;
            
            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString() ;

        private:
            std::string name_ ;
            DataType dt_ ;
            double number_ ;
            std::string str_ ;
            bool boolvalue_ ;
        };
    }
}
