#include <Subsystem.h>
#include <rev/ColorSensorV3.h>

namespace xero 
{
    namespace motorcontroller
    {
        class ColorSensorSubsystem : public xero::base::Subsystem
        {
        public:
            ColorSensorSubsystem(Subsystem *parent) ;
            virtual ~ColorSensorSubsystem() ;
            
            virtual void computeMyState() ;
            
        private:
            frc::Color color_ ;
            std::shared_ptr<rev::ColorSensorV3> sensor_ ;
        } ;
    }
}