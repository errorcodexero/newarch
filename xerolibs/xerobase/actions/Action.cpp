#include "Action.h"
#include "basegroups.h"

using namespace xero::misc;

namespace xero
{
    namespace base
    {
        uint32_t Action::global_action_id_ = 0;

        Action::Action(MessageLogger &logger) : logger_(logger)
        {
            action_id_ = global_action_id_++;            
            is_done_ = false;
        }

        Action::~Action()
        {            
        }

        void Action::start()
        {
            setNotDone();
            logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS);
            logger_ << "starting action: id " << action_id_ << ", action " << toString();
            logger_.endMessage();
        }

        void Action::run()
        {
            if (is_done_ != false)
            {
                logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS);
                logger_ << "action run called illegally: id " << action_id_ << ", action " << toString();
                logger_.endMessage();                
            }
        }

        void Action::cancel()
        {
            logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS);
            logger_ << "canceling action: id " << action_id_ << ", action " << toString();
            logger_.endMessage();              
        }

        void Action::setDone()
        {
            logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS);
            logger_ << "completing action: id " << action_id_ << ", action " << toString();
            logger_.endMessage();            
            is_done_ = true;
        }
    }
}