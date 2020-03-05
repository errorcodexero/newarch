#include "DroidLimeLight.h"
#include "droidids.h"
#include <Droid.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {

        std::vector<std::string> cols_ =
        {
            "time", 
            "ty", "angle", "lost"
        } ;

        DroidLimeLight::DroidLimeLight(Subsystem *parent) : LimeLight(parent) {
            Robot &robot = getRobot();
            camera_angle_ = robot.getSettingsParser().getDouble("droidvision:camera_angle") ;
            camera_height_ = robot.getSettingsParser().getDouble("droidvision:camera_target_height") ;
            plotid_ = initPlot("LimeLight") ;
            distance_ = 0 ;
            sample_time_ = 0;

            setCamMode(LimeLight::camMode::VisionProcessor);
            setLedMode(LimeLight::ledMode::ForceOff);
        }

        DroidLimeLight::~DroidLimeLight() {            
        }
        
        void DroidLimeLight::init(LoopType lt) {
        }

        void DroidLimeLight::reset() {
        }

        void DroidLimeLight::computeState() {
            LimeLight::computeState() ;

            if (isLimeLightPresent() && isTargetPresent()) 
            {
                //
                // Valid target, use the angle method for computing distance based on the
                // limelight documentation
                //
                distance_ = camera_height_ / std::tan(xero::math::deg2rad(camera_angle_ + getTY())) ;
                yaw_ = getTX() ;
                sample_time_ = getRobot().getTime() - getLatency();
            }
            
            frc::SmartDashboard::PutNumber("Distance", distance_) ;
            frc::SmartDashboard::PutNumber("YAW", yaw_) ;

#ifdef PLOT_LIMELIGHT
            if (getRobot().IsEnabled() && getRobot().IsAutonomous())
            {
                std::vector<double> data ;
                data.push_back(getRobot().getTime()) ;
                data.push_back(getTY()) ;
                data.push_back(distance_) ;
                data.push_back(isTargetPresent()) ;
                addPlotData(plotid_, data) ;
            }
#endif
        }
    }
}