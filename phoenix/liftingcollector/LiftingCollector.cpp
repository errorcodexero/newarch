#include "LiftingCollector.h"
#include "lifter/Lifter.h"
#include "lifter/LifterGoToHeightAction.h"
#include "collector/Collector.h"
#include "collector/CollectCubeAction.h"
#include "LiftingCollectCubeAction.h"
#include <ActionSequence.h>
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        LiftingCollector::LiftingCollector(Robot &robot) : Subsystem(robot, "liftingcollector") {

            lifter_ = std::make_shared<Lifter>(robot) ;
            addChild(lifter_) ;
            lifter_->createNamedSequences(lifter_) ;
            
            collector_ = std::make_shared<Collector>(robot) ;
            addChild(collector_) ;
            collector_->createNamedSequences(collector_) ;
        }

        LiftingCollector::~LiftingCollector() {
        }

        void LiftingCollector::createNamedSequences(SubsystemPtr sub) {
            std::shared_ptr<ActionSequence> list_p = std::make_shared<ActionSequence>(sub->getRobot().getMessageLogger(), "collectcube") ;
            list_p->pushSubActionPair(lifter_, std::make_shared<LifterGoToHeightAction>(*lifter_, "lifter:height:floor")) ;
            list_p->pushSubActionPair(collector_, std::make_shared<CollectCubeAction>(*collector_)) ;
            list_p->pushSubActionPair(lifter_, std::make_shared<LifterGoToHeightAction>(*lifter_, "lifter:height:exchange")) ;
            defineNamedSequence("collectCube", list_p) ; 
        }

        bool LiftingCollector::canAcceptAction(ActionPtr action) {
            auto ldir_p = std::dynamic_pointer_cast<LiftingCollectCubeAction>(action) ;
            if (ldir_p == nullptr)
                return false ;

            return true ;
        }

        void LiftingCollector::computeState() {
            lifter_->computeState() ;
            collector_->computeState() ;
        }
    }
}