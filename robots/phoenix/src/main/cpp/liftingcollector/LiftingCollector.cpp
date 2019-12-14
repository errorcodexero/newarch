#include "LiftingCollector.h"
#include "phlifter/Lifter.h"
#include "phlifter/LifterGoToHeightAction.h"
#include "collector/Collector.h"
#include "collector/CollectCubeAction.h"
#include "LiftingCollectCubeAction.h"
#include <actions/SequenceAction.h>
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        LiftingCollector::LiftingCollector(Subsystem *parent) : Subsystem(parent, "liftingcollector") {
            lifter_ = std::make_shared<Lifter>(parent) ;
            addChild(lifter_) ;
            lifter_->createNamedSequences() ;
            
            collector_ = std::make_shared<Collector>(parent) ;
            addChild(collector_) ;
            collector_->createNamedSequences() ;
        }

        LiftingCollector::~LiftingCollector() {
        }

        void LiftingCollector::createNamedSequences() {
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