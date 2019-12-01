#include "actions/SequenceAction.h"
#include "basegroups.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {
        SequenceAction::SequenceAction(xero::misc::MessageLogger &logger) : logger_(logger) 
        {
            isDone_ = false;
            isCancel_ = false ;
            group_ = MSG_GROUP_ACTIONS ;
        }

        void SequenceAction::pushAction(ActionPtr action)
        {
            actions_.push_back(action);
        }

        void SequenceAction::start()
        {
            index_ = -1 ;
        }

        void SequenceAction::startNextAction()
        {
            index_++;
            if (static_cast<size_t>(index_) < actions_.size())
            {
                assert(group_ != 0) ;
                logger_.startMessage(MessageLogger::MessageType::debug, group_) ;
                logger_ << "Actions: starting " << index_ << " of " << static_cast<int>(actions_.size() - 1) ;
                logger_ << " '" << actions_[index_]->toString() << "'" ;
                logger_.endMessage() ;
                actions_[index_]->start();
                frc::SmartDashboard::PutString("Step", actions_[index_]->toString()) ;
            }
            else {
                //
                // We have reached the end of the sequence
                //
                isDone_ = true ;
            }
        }

        void SequenceAction::run()
        {
            if (isCancel_)
            {
                //
                // We are trying to cancel, so we are waiting on the
                // current action to complete.  We have already asked the
                // current action to cancel.
                //
                if (index_ >= 0 && index_ <= static_cast<int>(actions_.size()))
                {
                    if (actions_[index_]->isDone())
                    {
                        isCancel_ = false ;
                        isDone_ = true ;
                    }
                }
                else
                {
                    isCancel_ = false ;
                    isDone_ = true ;
                }
            }
            else if (!isDone_)
            {
                //
                // We are actively running if we get here
                //
                while (1)
                {
                    if (index_ == static_cast<int>(actions_.size()))
                        break ;

                    if (index_ == -1 || actions_[index_]->isDone())
                    {
                        if (index_ != -1) {
                            logger_.startMessage(MessageLogger::MessageType::debug, group_) ;
                            logger_ << "Actions: completed " << index_ << " of " << static_cast<int>(actions_.size() - 1) ;
                            logger_ << " '" << actions_[index_]->toString() << "'" ;
                            logger_.endMessage() ;
                        }
                        
                        startNextAction();
                        if (static_cast<size_t>(index_) == actions_.size())
                            break;
                    };
                    actions_[index_]->run();
                    if (!actions_[index_]->isDone())
                    {
                        break;
                    }
                }
            }
        }

        bool SequenceAction::isDone()
        {
            return isDone_ ;
        }

        void SequenceAction::cancel()
        {
            isCancel_ = true;
            if (index_ != -1 && index_ < static_cast<int>(actions_.size()))
                actions_[index_]->cancel() ;
        }

        std::string SequenceAction::toString()
        {
            std::string result ;
            result += " [" ;
            for(size_t i = 0 ; i < actions_.size() ; i++) {
                auto ptr = actions_[i] ;
                if (i != 0)
                    result += "," ;
                result += actions_[i]->toString() ;
            }
            result += "]" ;
            return result ;
        }

        void SequenceAction::pushSubActionPair(SubsystemPtr subsystem, ActionPtr action, bool block)
        {
            auto p = std::make_shared<DispatchAction>(subsystem, action, block);
            pushAction(p);
        }

    } // namespace base
} // namespace xero
