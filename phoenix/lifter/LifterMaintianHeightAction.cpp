#include "LifterMaintainHeightAction.h"

namespace xero {
    namespace phoenix {
        LifterMaintainHeightAction::LifterMaintainHeightAction(Lifter& lifter, Lifter::LIFTER_GEAR value) : LifterAction(lifter) {
            value_ = value;
            
            climb_delta_= getLifter().getRobot().getSettingsParser().getInteger("lifter:climb:delta");
        }

        void LifterMaintainHeightAction::start() {
            state_ = LIFTER_STATE::HOLDING;
            encoder_value_target_ = getLifter().encoder_ticks_;
            getLifter().setBrake(true);
        }
        void LifterMaintainHeightAction::run() {
                
        }
        bool LifterMaintainHeightAction::isDone() {
            return false;

        }
        void LifterMaintainHeightAction::cancel() {

        }
        std::string LifterMaintainHeightAction::toString() {
            return action_name_;
        }
        const std::string LifterMaintainHeightAction::action_name_("LifterMaintainHeight");
    }
}