#include "OIModel.h"

#include <frc/RobotSimBase.h>
#include <ncurses.h>

using namespace frc ;
using namespace xero::misc ;

namespace xero {
    namespace sim {
        namespace phaser {
            OIModel::OIModel(RobotSimBase &simbase) : SubsystemModel(simbase, "oi") {
                ds_ = nullptr ;
            }

            OIModel::~OIModel() {               
            }

            void OIModel::processEvent(const std::string &event, int value) {
                if (event == "automode") {
                    double v = static_cast<double>(value) / 100.0 ;
                    setAxis(2, 6, v) ;                  
                }
                else if (event == "tracking_viewing_nothing_switch") {
                    double v = static_cast<double>(value) / 100.0 ;
                    setAxis(2, 3, v) ;
                }
                else {
                    SettingsParser &settings =  getSimulator().getSettingsParser() ;
                    std::string name = "oi:" + event ;
                    if (settings.isDefined(name)) {
                        int button = settings.getInteger(name) ;
                        setButton(2, button, (value ? true : false)) ;
                    }
                }
            }

            std::string OIModel::toString() {
                std::string result("OI: ") ;
                return result ;
            }

            void OIModel::run(double dt) {
            }

            void OIModel::inputChanged(SimulatedObject *obj) {              
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
            }    

            void OIModel::addDevice(frc::DriverStation *ds) {
                ds_ = ds ;
            }     

            void OIModel::setButton(int which, int button, bool value) {
                assert(ds_ != nullptr) ;
                ds_->getStick(which).setButtonValue(button, value) ;
            }

            void OIModel::setAxis(int which, int axis, double value) {  
                assert(ds_ != nullptr) ;                
                ds_->getStick(which).setAxisValue(axis, value) ;            
            }

            void OIModel::setPOV(int which, int pov, int value) {               
                assert(ds_ != nullptr) ;                
                ds_->getStick(which).setPOVValue(pov, value) ;
            }

            bool OIModel::getButton(int which, int button) {
                if (ds_ == nullptr)
                    return false ;

                return ds_->getStick(which).getButtonValue(button) ;
            }

            double OIModel::getAxis(int which, int axis) {
                if (ds_ == nullptr)
                    return 0.0 ;
                return ds_->getStick(which).getAxisValue(axis) ;
            }

            int OIModel::getPOV(int which, int pov) {
                if (ds_ == nullptr)
                    return 0 ;
                    
                return ds_->getStick(which).getPOVValue(pov) ;
            }
        }
    }
}
