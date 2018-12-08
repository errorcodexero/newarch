#include "LiftingCollectCubeAction.h"
#include "LiftingCollector.h"
#include "collector/Collector.h"
#include "collector/CollectCubeAction.h"
#include "lifter/Lifter.h"
#include "lifter/LifterGoToHeightAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        std::string LiftingCollectCubeAction::action_name("LiftingCollectCube") ;

        LiftingCollectCubeAction::LiftingCollectCubeAction(LiftingCollector &col, double height) : LiftingCollectorAction(col) {            
            drive_height_ = height ;
        }

        LiftingCollectCubeAction::LiftingCollectCubeAction(LiftingCollector &col, const std::string &height) : LiftingCollectorAction(col) {            
            drive_height_ = col.getRobot().getSettingsParser().getDouble(height) ;
        }

        LiftingCollectCubeAction::~LiftingCollectCubeAction() {
        }

        void LiftingCollectCubeAction::start() { 
            ActionPtr go = std::make_shared<LifterGoToHeightAction>(*getLiftingCollector().getLifter(), "lifter:height:floor") ;
            getLiftingCollector().getLifter()->setAction(go) ;

            collect_action_ = std::make_shared<CollectCubeAction>(*getLiftingCollector().getCollector()) ;
            getLiftingCollector().getCollector()->setAction(collect_action_) ;
        }

        void LiftingCollectCubeAction::run() {            
        }

        bool LiftingCollectCubeAction::isDone() {
            return getLiftingCollector().getCollector()->isDone() ;
        }

        void LiftingCollectCubeAction::cancel() {            
        }
    }
}