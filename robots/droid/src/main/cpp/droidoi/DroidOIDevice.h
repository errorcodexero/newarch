#pragma once

#include <oi/OIDevice.h> 
#include <actions/Action.h>
#include <actions/SequenceAction.h>

namespace xero {
    namespace droid {
        class DroidOISubsystem ;

        class DroidOIDevice : public xero::base::OIDevice {
        public:
            DroidOIDevice(DroidOISubsystem &sub, int index) ;
            virtual ~DroidOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::SequenceAction &seq) ;

            virtual void init() ;

        private:
            enum class RobotMode
            {
                Shooting,
                Collecting
            } ;

        private:
            void bindOI() ;
            
        private:
            RobotMode mode_ ;
            bool collecting_ ;
            bool shooting_;

            size_t automode_ ;
            size_t collect_shoot_mode_ ;
            size_t shoot_on_off_ ;
            size_t collect_on_off_ ;

            xero::base::ActionPtr follow_target_ ;
            xero::base::ActionPtr prepare_to_emit_ ;
            xero::base::ActionPtr prepare_to_receive_ ;
            xero::base::ActionPtr start_collect_ ;
            xero::base::ActionPtr stop_collect_;
            xero::base::ActionPtr shoot_;
        } ;
    }
}
