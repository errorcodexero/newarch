#pragma once

#include <oi/OIDevice.h> 
#include <actions/Action.h>
#include <actions/SequenceAction.h>
#include <queue>

namespace xero {
    namespace droid {
        class DroidOISubsystem ;

        class DroidOIDevice : public xero::base::OIDevice {
        public:
            DroidOIDevice(DroidOISubsystem &sub, int index) ;
            virtual ~DroidOIDevice() ;

            virtual void init() ;

            virtual int getAutoModeSelector() ;

            virtual void generateActions(xero::base::SequenceAction &seq) ;

        private:
            void initialize() ;

            void clearQueue() {
                while(conveyorActionQueue.empty())
                    conveyorActionQueue.pop() ;
            }
        private:
            bool flag_coll_v_shoot_ ;
            bool flag_collect_ ;
            bool flag_shoot_ ;
            
            std::queue<xero::base::ActionPtr> conveyorActionQueue ;

            size_t automode_ ;

            size_t coll_v_shoot_ ;
            size_t collect_ ;
            size_t shoot_ ;

            xero::base::ActionPtr intake_collect_ ;
            xero::base::ActionPtr intake_retract_ ;
            xero::base::ActionPtr queue_collect_ ;
            xero::base::ActionPtr queue_shoot_ ;
            xero::base::ActionPtr queue_prep_collect_ ;
            xero::base::ActionPtr queue_prep_shoot_ ;
            xero::base::ActionPtr fire_yes_ ;
            //xero::base::ActionPtr fire_no_ ;

        } ;
    }
}
