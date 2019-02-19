#include "LifterHoldHeightAction.h"
#include "Lifter.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::list<std::string> LifterHoldHeightAction::plot_columns_ = {
            "time", 
            "tpos", "apos", "out"
        } ;

        LifterHoldHeightAction::LifterHoldHeightAction(Lifter &lifter, double target) : LifterAction(lifter) {
            target_ = target ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            pid_ctrl_.initFromSettingsExtended(lifter.getRobot().getSettingsParser(), "lifter:hold") ;            
        }

        LifterHoldHeightAction::LifterHoldHeightAction(Lifter &lifter, const std::string &name) : LifterAction(lifter) {
            target_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            pid_ctrl_.initFromSettingsExtended(lifter.getRobot().getSettingsParser(), "lifter:hold") ;
        }

        LifterHoldHeightAction::~LifterHoldHeightAction() {
        }

        void LifterHoldHeightAction::start() {
            Lifter &lifter = getLifter() ;            
            if (!lifter.isCalibrated()) {
                MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "requested LifterGoToHeightAction when the lifter was not calibrated" ;
                logger.endMessage() ;
                is_done_ = true ;
            }           
            else {
                is_done_ = false ;
                start_ = lifter.getRobot().getTime() ;
                plotid_ = getLifter().getRobot().startPlot("LifterHoldHeight", plot_columns_) ;
                index_ = 0 ;                
            } 
        }

        void LifterHoldHeightAction::run() {
            Lifter &lifter = getLifter() ;               
            double dist = target_ - lifter.getHeight() ;
            double out = pid_ctrl_.getOutput(0, dist, 0, getLifter().getRobot().getDeltaTime()) ;


            lifter.setMotorPower(out) ;

            double elapsed = lifter.getRobot().getTime() - start_ ;
            lifter.getRobot().addPlotData(plotid_, index_, 0, elapsed) ;
            lifter.getRobot().addPlotData(plotid_, index_, 1, target_) ;
            lifter.getRobot().addPlotData(plotid_, index_, 2, lifter.getHeight()) ;
            lifter.getRobot().addPlotData(plotid_, index_, 3, out) ;
            index_++ ;
        }

        bool LifterHoldHeightAction::isDone() {
            return is_done_ ;
        }

        void LifterHoldHeightAction::cancel() {
            is_done_ = true ; 
        }

        std::string LifterHoldHeightAction::toString() {
            std::string result = "LifterHoldHeight " + std::to_string(target_) ;
            return result ;
        }
    }
}