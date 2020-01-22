#include <WestCoastDrive.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        WestCoastDrive::WestCoastDrive(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "westcoastdrive", inst)
        {
        }

        bool WestCoastDrive::create()
        {
            try
            {
                left_motor_ = std::make_shared<SimulatedMotor>(*this, "hw:left:motor") ;
                right_motor_ = std::make_shared<SimulatedMotor>(*this, "hw:right:motor") ;                
            }
            catch(const std::runtime_error &e)
            {
                return false;
            }

            left_enc_[0] = getInteger("hw:left:encoder:1") ;

            return true ;
        }

        WestCoastDrive::~WestCoastDrive()
        {
        }

        void WestCoastDrive::run()
        {
        }
    }
}