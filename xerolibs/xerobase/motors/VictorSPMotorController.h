#pragma once
#include "MotorController.h"
#include <frc/VictorSP.h>
#include <list>
#include <memory>
#include <cassert>


namespace xero {
    namespace base {
        class VictorSPMotorController: public MotorController {
        public:
            typedef std::shared_ptr<frc::VictorSP> MotorPtr;
        
            /// \brief Creates a new Spark Max motor object
            /// \param pwmID The motor ID.
            VictorSPMotorController(int pwmID);

            /// \brief destroys the motor controller
            virtual ~VictorSPMotorController() ;

            /// \brief set the motor power to the given percentage
            /// \param percent the power to apply to the motor
            virtual void set(double percent);

            /// \brief set the invertec flag for the motor.
            /// If this, the direction of the motor is reversed from its default
            /// \param inverted if true, invert the direction of the motor
            virtual void setInverted(bool inverted);

            /// \brief set the neutral mode for the motor.
            /// Generally this is coast mode or brake mode
            /// \param neutralMode the mode for the motor when in neutral
            virtual void setNeutralMode(NeutralMode neutralMode);

            /// \brief follow another VictprSPMotorController
            /// \param motor the motor to follow
            /// \param invert if true, invert the power when following
            virtual void follow(std::shared_ptr<MotorController> motor, bool invert = false);

            /// \brief reapply the inverted status
            virtual void reapplyInverted();

            /// \brief return a string describing the motor type
            /// This method returns the string "victorsp".
            /// \returns a string describing the motor type
            virtual std::string getType() {
                std::string type("victorsp") ;
                return type ;
            }

            /// \brief return the underlying motor controller object
            /// \returns the underlying motor controller object            
            MotorPtr getVictorSPMotorPtr() {
                return motor_ ;
            }

        protected:
            /// \brief add the given motor as a follower
            /// \param motor the motor to folow this one
            void addAsFollower(VictorSPMotorController *motor) {
                followers_.push_back(motor) ;
            }

        private:
            MotorPtr motor_;
            bool isInverted_;
            std::shared_ptr<VictorSPMotorController> following_ ;
            std::list<VictorSPMotorController *> followers_ ;
        };
    }
}