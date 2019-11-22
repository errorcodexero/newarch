#include "RanseurVision.h"
#include <frc/RobotSimBase.h>
#include <networktables/NetworkTableInstance.h>
#include <xeromath.h>

using namespace frc ;
using namespace xero::sim ;

namespace xero {
    namespace sim {
        namespace ranseur {

            RanseurVision::RanseurVision(RobotSimBase &simbase, TankDriveModel &db, TubManipulatorModel &tubmanip) 
                        : SubsystemModel(simbase, "vision"), tankdrive_(db), tubmanipulator_(tubmanip) {
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
                    lines.push_back("  camx " + std::to_string(rx_)) ;
                    lines.push_back("  camy " + std::to_string(ry_)) ;                    
                    lines.push_back("  dist " + std::to_string(dist_)) ;                    
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

                    double dcamoffset = 11.0 ;              // The robot length, assuming the camera

                    double fov = 90.0 ;

                    //
                    // We assume the camera is mounted on the front of the robot
                    //
                    double cx = tankdrive_.getXPos() ;
                    double cy = tankdrive_.getYPos() ;
                    double ra = tankdrive_.getAngle() ;

                    rx_ = cx + cos(xero::math::deg2rad(ra)) * dcamoffset ;
                    ry_ = cy + sin(xero::math::deg2rad(ra)) * dcamoffset ;

                    double targetheight = 3.0 ;
                    double cameraheight = 8.0 ;

                    double phit = xero::math::rad2deg(atan2(tubypos_ - ry_, tubxpos_ - rx_)) ;
                    tx_ = phit - ra ;

                    dist_ = std::sqrt((rx_ - tubxpos_) * (rx_ - tubxpos_) + (ry_ - tubypos_) * (ry_ - tubypos_)) ;

                    if (std::fabs(dist_) < dmindist)
                    {
                        tubxpos_ = -1000.0 ;
                        tubypos_ = -1000.0 ;
                        tubmanipulator_.hasTub(true) ;
                    }

                    if (dist_ > dmaxdist || dist_ < dmindist || tx_ < -fov / 2.0 || tx_ > fov / 2.0)
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
                        ta_ = (dmaxdist - dmindist) / (dist_ - dmindist) * (dminpcnt - dmaxpcnt) + dminpcnt ;
                        ty_ = xero::math::rad2deg(atan2(targetheight - cameraheight, dist_)) ;
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
