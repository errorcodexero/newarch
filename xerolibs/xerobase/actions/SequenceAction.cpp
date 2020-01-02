#include "actions/SequenceAction.h"
#include "basegroups.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {
        SequenceAction::SequenceAction(xero::misc::MessageLogger &logger) : CompositeAction(logger)
        {
            isCancel_ = false ;
            group_ = MSG_GROUP_ACTIONS ;
        }

        void SequenceAction::pushAction(ActionPtr action)
        {
            actions_.push_back(action);
        }

        void SequenceAction::pushSubActionPair(SubsystemPtr subsystem, ActionPtr action, bool block)
        {
            auto p = std::make_shared<DispatchAction>(subsystem, action, block);
            pushAction(p);
        }        

        void SequenceAction::start()
        {
            CompositeAction::start();
            index_ = -1 ;
            isCancel_ = false ;
        }

        void SequenceAction::startNextAction()
        {
            index_++;
            if (static_cast<size_t>(index_) < actions_.size())
            {
                actions_[index_]->start();
                frc::SmartDashboard::PutString("Step", actions_[index_]->toString()) ;
            }
            else {
                setDone();
            }
        }

        void SequenceAction::run()
        {
            CompositeAction::run();
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
                        setDone();
                    }
                }
                else
                {
                    //
                    // The current action finished normally, we are still done
                    //
                    isCancel_ = false ;
                    setDone();
                }
            }
            else
            {
                //
                // We are actively running if we get here
                //
                while (1)
                {
                    if (index_ == static_cast<int>(actions_.size()))
                    {
                        setDone();
                        break ;
                    }

                    if (index_ == -1 || actions_[index_]->isDone())
                    {
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

        void SequenceAction::cancel()
        {
            Action::cancel();
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


    } // namespace base
} // namespace xero
