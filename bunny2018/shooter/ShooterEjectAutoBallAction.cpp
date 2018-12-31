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
            ShooterEjectOneBallAction::start() ;
            
            base_location_ = db_->getDist() ;
            filled_.resize(crates_.size()) ;
            std::fill(filled_.begin(), filled_.end(), false) ;

            std::cout << "Base Value At Start " << base_location_ << std::endl ;
        }

        size_t ShooterEjectAutoBallAction::getClosestCrate(double pos, bool fwd) {
            size_t ret = std::numeric_limits<size_t>::max() ;

            if (fwd) {
                for(size_t index = 0 ; index < crates_.size() ; index++) {
                    double loc = crates_[index] ;
                    if (loc > pos) {
                        ret = index ;
                        break ;                     
                    }
                }
            }
            else {
                size_t index = crates_.size() - 1 ;
                do {
                    double loc = crates_[index] ;
                    if (loc < pos) {
                        ret = index ;
                        break ;
                    }
                } while (index-- != 0) ;
            }

            return ret ;
        }

        bool ShooterEjectAutoBallAction::ejectTrigger() {
            double loc ;
            double offloc ;
            size_t index = 0 ;
            double curpos = db_->getDist() - base_location_ ;
            bool ret = false ;

            // std::cout << "dist " << db_->getDist() << " " << base_location_ << std::endl ;

            if (db_->getVelocity() > 0) {
                index = getClosestCrate(curpos, true) ;             
                if (index < crates_.size()) {

                    loc = crates_[index] ;
                    offloc = loc - offset_ ;
                    if (filled_[index] == false && curpos >= offloc - window_ && curpos <= offloc + window_) {
                        filled_[index] = true ;
                        ret = true ;
                    }
                }
            }
            else {
                index = getClosestCrate(curpos, false) ;
                if (index < crates_.size()) {
                    loc = crates_[index] ;
                    offloc = loc + offset_ ;
                    if (filled_[index] == false && curpos >= offloc - window_ && curpos <= offloc + window_) {
                        filled_[index] = true ;
                        ret = true ;
                    }
                }
            }

            auto &logger = getSubsystem().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SHOOTER_VERBOSE) ;
            logger << "ejectTrigger:" ;
            logger << " curpos " << curpos ;
            logger << ", index " << index ;
            if (index < crates_.size())
                logger << ", filled " << filled_[index] ;
            logger << ", velocity " << db_->getVelocity() ;
            logger << ", loc " << loc ;
            logger << ", offsetloc " << offloc ;
            logger << ", check " << offloc - window_ << " " << offloc + window_ ;
            logger << ", offset " << offset_ ;
            logger << ", window " << window_ ;
            logger << ", ret " << ret ;
            logger.endMessage() ;

            return ret ;
        }

        bool ShooterEjectAutoBallAction::repeatEjectOne() {
            return true ;
        }
    }
}