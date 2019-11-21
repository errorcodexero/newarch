#include "RanseurVision.h"
#include <frc/RobotSimBase.h>
#include <networktables/NetworkTableInstance.h>

using namespace frc ;
using namespace xero::sim ;

namespace xero {
    namespace sim {
        namespace ranseur {

            RanseurVision::RanseurVision(RobotSimBase &simbase, TankDriveModel &db) : SubsystemModel(simbase, "vision"), tankdrive_(db) {
            }

            RanseurVision::~RanseurVision() {
            }

            void RanseurVision::generateDisplayInformation(std::list<std::string> &lines) {
            }

            std::string RanseurVision::toString() {
                std::string result ;
                return result ;
            }

            bool RanseurVision::processEvent(const std::string &name, int value) {
                bool ret = false ;
                if (name == "tubxpos") {
                    tubxpos_ = (double)(value / 100.0) ;
                    ret = true ;
                }
                else if (name == "tubypos") {
                    tubypos_ = (double)(value / 100.0) ;
                    ret = true ;
                }

                return ret ;
            }            

            void RanseurVision::compute() {
                tv_ = 0 ;
                tx_ = 0.0 ;
                ty_ = 0.0 ;
                ta_ = 0.0 ;
                ts_ = 0.0 ;
                tl_ = 0.0 ;
                tshort_ = 0.0 ;
                tlong_ = 0.0 ;
                thor_ = 0.0 ;
                tvert_ = 0.0 ;
                getpipe_ = 0 ;
                while (camtran_.size() < 6)
                    camtran_.push_back(0.0) ;
            }

            void RanseurVision::run(double dt) {
                compute() ;

                nt::NetworkTableInstance ntinst = nt::NetworkTableInstance::GetDefault() ;
                auto table = ntinst.GetTable("limelight") ;

                table->PutNumber("tv", tv_) ;
                table->PutNumber("tx", tx_) ;
                table->PutNumber("ty", ty_) ;
                table->PutNumber("ta", ta_) ;
                table->PutNumber("ts", ts_) ;
                table->PutNumber("tl", tl_) ;
                table->PutNumber("tshort", tshort_) ;
                table->PutNumber("tlong", tlong_) ;
                table->PutNumber("thor", thor_) ;
                table->PutNumber("tvert", tvert_) ;
                table->PutNumber("getpipe", getpipe_) ;
                table->PutNumberArray("camtran", camtran_) ;
            }

            void RanseurVision::inputChanged(SimulatedObject *obj) {
            }
        }
    }
}
