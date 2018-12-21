#include "ShooterEjectAutoBallAction.h"
#include "bunnyids.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
        ShooterEjectAutoBallAction::ShooterEjectAutoBallAction(Shooter& shooter) : ShooterEjectOneBallAction(shooter) {
            db_ = shooter.getRobot().getDriveBase() ;

			crate_width_ = shooter.getRobot().getSettingsParser().getDouble("automode:1:crate_width") ;
			offset_ = shooter.getRobot().getSettingsParser().getDouble("automode:1:offset") ;
			window_ = shooter.getRobot().getSettingsParser().getDouble("automode:1:window") ;
        }

        ShooterEjectAutoBallAction::~ShooterEjectAutoBallAction() {
        }

        void ShooterEjectAutoBallAction::start() {
            base_location_ = db_->getDist() ;
            filled_.resize(crates_.size()) ;
            std::fill(filled_.begin(), filled_.end(), false) ;
        }

		double ShooterEjectAutoBallAction::getClosestCrate(double pos, bool fwd) {
			double ret = std::numeric_limits<double>::max() ;

			if (fwd) {
				for(size_t index = 0 ; index < crates_.size() ; index++) {
					double loc = crates_[index] ;
					if (loc > pos) {
						ret = loc ;
						break ;						
					}
				}
			}
			else {
				size_t index = crates_.size() - 1 ;
				do {
					double loc = crates_[index] ;
					if (loc < pos) {
						ret = loc ;
						break ;
					}
				} while (index-- != 0) ;
			}

			return ret ;
		}

        bool ShooterEjectAutoBallAction::ejectTrigger() {
			double loc ;
			double offloc ;
			double curpos = db_->getDist() - base_location_ ;

			if (db_->getVelocity() > 0) {
				loc = getClosestCrate(curpos, true) ;
				offloc = loc - offset_ ;
				if (curpos >= offloc - window_ && curpos <= offloc + window_)
					return true ;
			}
			else {
				loc = getClosestCrate(curpos, false) ;
				offloc = loc + offset_ ;
				if (curpos >= offloc - window_ && curpos <= offloc + window_)
					return true ;
			}

			auto &logger = getSubsystem().getRobot().getMessageLogger() ;
			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SHOOTER_VERBOSE) ;
			logger << "ejectTrigger:" ;
			logger << " curpos " << curpos ;
			logger << ", velocity " << db_->getVelocity() ;
			logger << ", loc " << loc ;
			logger << ", offsetloc " << offloc ;
			logger << ", check " << loc - window_ << " " << loc + window_ ;
			logger << ", offset " << offset_ ;
			logger << ", window " << window_ ;
			logger.endMessage() ;

            return false ;
        }

        bool ShooterEjectAutoBallAction::repeatEjectOne() {
            return true ;
        }
    }
}