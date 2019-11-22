#include "RanseurVision.h"
#include <frc/RobotSimBase.h>
#include <networktables/NetworkTableInstance.h>
#include <xeromath.h>

using namespace frc ;
using namespace xero::sim ;

namespace xero {
    namespace sim {
        namespace ranseur {

            RanseurVision::RanseurVision(RobotSimBase &simbase, TankDriveModel &db) : SubsystemModel(simbase, "vision"), tankdrive_(db) {
                tubxpos_set_ = false ;
                tubypos_set_ = false ;
            }

            RanseurVision::~RanseurVision() {
            }

            void RanseurVision::generateDisplayInformation(std::list<std::string> &lines) {
                std::string line ;

                if (tubxpos_set_ && tubypos_set_)
                {
                    lines.push_back("  tubx " + std::to_string(tubxpos_)) ;
                    lines.push_back("  tuby " + std::to_string(tubypos_)) ;
                    lines.push_back("  tv " + std::to_string(tv_)) ;
                    lines.push_back("  tx " + std::to_string(tx_)) ;
                    lines.push_back("  ty " + std::to_string(ty_)) ;
                    lines.push_back("  ta " + std::to_string(ta_)) ;                    
                }
            }

            std::string RanseurVision::toString() {
                std::string result ;
                return result ;
            }

            bool RanseurVision::processEvent(const std::string &name, int value) {
                bool ret = false ;
                if (name == "tubxpos") {
                    tubxpos_ = (double)(value / 100.0) ;
                    tubxpos_set_ = true ;
                    ret = true ;
                }
                else if (name == "tubypos") {
                    tubypos_ = (double)(value / 100.0) ;
                    tubypos_set_ = true ;
                    ret = true ;
                }

                return ret ;
            }            

            void RanseurVision::compute() {
                nt::NetworkTableInstance ntinst = nt::NetworkTableInstance::GetDefault() ;
                auto table = ntinst.GetTable(table_name) ;

                while (camtran_.size() < 6)
                    camtran_.push_back(0.0) ;

                table->PutNumber("camMode", 0.0) ;

                if (std::fabs(table->GetNumber("camMode", 1.0)) > 0.01 || !tubxpos_set_ || !tubypos_set_)
                {
                    // Camera is in driver mode
                    tv_ = 0 ;
                }
                else
                {
                    double dmindist = 6.0 ;                 // Min target distance is 6 inches
                    double dminpcnt = 0.5 ;                 // At min distance, the target fills 50% of image

                    double dmaxdist = 10.0 * 12.0 ;         // Max target distance is 10 feet (120 inches)
                    double dmaxpcnt = 0.01 ;                // At max distance, the target fills 1 % of the image

                    double fov = 90.0 ;

                    //
                    // We assume the camera is mounted on the front of the robot
                    //
                    double rx = tankdrive_.getXPos() ;
                    double ry = tankdrive_.getYPos() ;
                    double ra = tankdrive_.getAngle() ;

                    double phit = xero::math::rad2deg(atan2(tubypos_ - ry, tubxpos_ - rx)) ;
                    tx_ = phit - ra ;
                    ty_ = -15.0 ;

                    double dist = std::sqrt((rx - tubxpos_) * (rx - tubxpos_) + (ry - tubypos_) * (ry - tubypos_)) ;
                    if (dist > dmaxdist || dist < dmindist || tx_ < -fov / 2.0 || tx_ > fov / 2.0)
                    {
                        //
                        // Either too close or too far
                        //
                        tv_ = 0 ;
                        ta_ = 0.0 ;
                        tx_ = 0.0 ;
                        ty_ = 0.0 ;
                    }
                    else
                    {
                        tv_ = 1 ;
                        ta_ = (dmaxdist - dmindist) / (dist - dmindist) * (dminpcnt - dmaxpcnt) + dminpcnt ;
                    }

                    ts_ = 0.0 ;
                    tl_ = 0.0 ;
                    tshort_ = 0.0 ;
                    tlong_ = 0.0 ;
                    thor_ = 0.0 ;
                    tvert_ = 0.0 ;
                    getpipe_ = 0 ;
                }
            }

            void RanseurVision::run(double dt) {
                compute() ;

                nt::NetworkTableInstance ntinst = nt::NetworkTableInstance::GetDefault() ;
                auto table = ntinst.GetTable(table_name) ;

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
