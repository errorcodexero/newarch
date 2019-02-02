#include "OIModel.h"

#include <frc/RobotSimBase.h>

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            OIModel::OIModel(RobotSimBase &simbase) : SubsystemModel(simbase, "oi") {
                ds_ = nullptr ;
            }

            OIModel::~OIModel() {               
            }

            void OIModel::init() {
                if (getSimulator().hasProperty("autovalue")) {
                    std::vector<double> values ;
                    const std::string &prop = getSimulator().getProperty("autovalue") ;
                    if (parseDoubleList(prop, values)) {
                        autovalue_ = values[0] ;
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
                auto &stick = ds_->getStick(2) ;
                //
                // Axis used to detect the OI
                //
                stick.setAxisValue(9, 1.0) ;

                //
                // Axis used to control the automode switch
                //
                stick.setAxisValue(6, autovalue_) ;

                //
                // The climb diabled button is true to start, disabling climb
                //
                stick.setButtonValue(15, false) ;
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
