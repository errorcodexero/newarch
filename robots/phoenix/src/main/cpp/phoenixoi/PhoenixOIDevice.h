#pragma once

#include <oi/OIDevice.h>
#include <Action.h>

namespace xero {
    namespace phoenix {
        class PhoenixOISubsystem ;

        class PhoenixOIDevice : public xero::base::OIDevice {
        public:
            PhoenixOIDevice(PhoenixOISubsystem &, int index) ;
            virtual ~PhoenixOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::SequenceAction &seq) ;

        private:
            void createActions() ;
            void initialize() ;
            
        private:
            size_t floor_ ;
            size_t exchange_ ;
            size_t switch_ ;
            size_t scale_ ;
            size_t collect_ ;
            size_t eject_ ;
            size_t drop_ ;
            size_t climb_ ;
            size_t wings_ ;
            size_t eject_slow_ ;
            size_t intake_auto_ ;
            size_t calibrate_ ;
            size_t lifter_up_ ;
            size_t lifter_down_ ;
            size_t climb_disabled_ ;
            size_t lifter_high_power_ ;

            size_t automode_ ;
            size_t grabber_auto_ ;
            size_t grabber_mode_ ;
            size_t intake_mode_ ;
            size_t collect_end_height_ ;   

            xero::base::ActionPtr to_floor_ ;
            xero::base::ActionPtr to_exchange_ ;
            xero::base::ActionPtr to_switch_ ;
            xero::base::ActionPtr to_scale_ ;
            xero::base::ActionPtr to_climb_ ;
            xero::base::ActionPtr lifter_up_low_power_ ;
            xero::base::ActionPtr lifter_down_low_power_ ;
            xero::base::ActionPtr lifter_up_high_power_ ;
            xero::base::ActionPtr lifter_down_high_power_ ;         
            xero::base::ActionPtr lifter_climb_up_ ;
            xero::base::ActionPtr lifter_climb_down_ ;
            xero::base::ActionPtr lifter_hold_climb_ ;
            xero::base::ActionPtr lifter_zero_ ;

            xero::base::ActionPtr lifter_calibrate_ ;
            xero::base::ActionPtr grabber_calibrate_ ;

            xero::base::ActionPtr grabber_stow_ ;
            xero::base::ActionPtr collect_cube_ ;
            xero::base::ActionPtr eject_normal_ ;
            xero::base::ActionPtr eject_medium_ ;
            xero::base::ActionPtr eject_slow_action_ ;                        

            bool lifter_stop_ ;

            bool prev_climb_enabled_ ;
        } ;
    }
}