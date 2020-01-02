#include "TubCollectorTubAction.h"
#include "TubCollector.h"
#include "TubCollectorDutyCycleAction.h"
#include "ranseurids.h"
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::vector<std::string> TubCollectorTubAction::cols_ = 
        {
            "time",
            "current",
            "state"
        } ;

        std::string TubCollectorTubAction::action_name("TubCollectorTubAction");

        TubCollectorTubAction::TubCollectorTubAction(TubCollector &col) : TubCollectorAction(col){
            speed_ = col.getRobot().getSettingsParser().getDouble("tubcollector:collect:power") ;
            delay_ = col.getRobot().getSettingsParser().getDouble("tubcollector:collect:duration") ;

            plotid_ = col.initPlot("tubcollect") ;
        }
        TubCollectorTubAction::~TubCollectorTubAction() {            
        }
        
        void TubCollectorTubAction::start() {
            TubCollectorAction::start();

            getTubCollector().startPlot(plotid_, cols_);
            getTubCollector().openHand() ;
            state_ = State::motors_on ;
            getTubCollector().setIntakePower(speed_) ;
        }

        void TubCollectorTubAction::run() {
            TubCollectorAction::run();

            if (state_ == State::motors_on)
            {
                if(getTubCollector().hasTub()) {
                    auto &logger = getTubCollector().getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TUBCOLLECTOR) ;
                    logger << "Detect tub, moving to next state" ;
                    logger.endMessage() ;
                    getTubCollector().closeHand() ;
                    state_ = State::closed_hand ;
                    start_ = getTubCollector().getRobot().getTime() ;
                }
            }
            else if(state_ == State::closed_hand) {
                if(getTubCollector().getRobot().getTime() - start_ > delay_) {
                    getTubCollector().setIntakePower(0.0) ;
                    state_ = State::done ;
                    setDone();
                    getTubCollector().endPlot(plotid_);
                }
            }

            std::vector<double> data ;
            data.push_back(getTubCollector().getRobot().getTime()) ;
            data.push_back(getTubCollector().getCollectPower()) ;
            data.push_back(getTubCollector().getState()) ;
            getTubCollector().addPlotData(plotid_, data) ;
        }

        void TubCollectorTubAction::cancel() {
            TubCollectorAction::cancel();
            getTubCollector().setIntakePower(0.0);
            getTubCollector().closeHand() ;
            state_ = State::done ;
        }
    }
}
