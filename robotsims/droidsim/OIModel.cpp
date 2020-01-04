#include "OIModel.h"

#include <frc/RobotSimBase.h>
#include <ncurses.h>

using namespace frc ;
using namespace xero::misc ;

namespace xero {
    namespace sim {
        namespace droid {
            OIModel::OIModel(RobotSimBase &simbase) : SubsystemModel(simbase, "oi") {
                ds_ = &frc::DriverStation::GetInstance() ;
            }

            OIModel::~OIModel() {               
            }

            bool OIModel::processEvent(const std::string &event, int value) {
                bool ret = false ;
                SettingsParser &settings =  getSimulator().getSettingsParser() ;
                std::string name = "oi:button:" + event ;
                if (settings.isDefined(name)) {
                    int button = settings.getInteger(name) ;
                    setButton(2, button, (value ? true : false)) ;
                    ret = true ;
                }
                else {
                    name = "oi:axis:" + event ;
                    if (settings.isDefined(name)) {
                        int axis = settings.getInteger(name) ;
                        double v = static_cast<double>(value) / 100.0 ;
                        setAxis(2, axis, v) ;
                        ret = true ;
                    }                        
                }

                return ret ;
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
