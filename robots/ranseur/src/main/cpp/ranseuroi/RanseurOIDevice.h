#pragma once

#include <oi/OIDevice.h> 
#include <SequenceAction.h>

namespace xero {
    namespace ranseur {
        class RanseurOISubsystem ;

        class RanseurOIDevice : public xero::base::OIDevice {
        public:
            enum class Direction
            {
                North = 0,
                South = 1,
                East = 2,
                West = 3,
            } ;

            enum class OperationMode
            {
                Auto,
                SemiAuto,
                Manual,
                Invalid
            } ;

        public:
            RanseurOIDevice(RanseurOISubsystem &sub, int index) ;
            virtual ~RanseurOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void init() ;
            virtual void generateActions(xero::base::SequenceAction &seq) ;

        private:
            void bindOI() ;
            void getTrackingMode() ;
            
            void setupVisionDetectors() ;

            void manuallyFinish(xero::base::SequenceAction &seq) ;

            std::string toString(OperationMode mode) ;

            void updateMode(OperationMode newmode); 

        private:
            
        private:
            size_t automode_ ;

            size_t tracking_manual_switch_ ;

            OperationMode mode_ ;
         
            bool field_relative_ ;

        } ;
    }
}