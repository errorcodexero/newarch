#include "message_logger.h"
#include "message_logger_dest.h"
#include <sstream>
#include <iostream>

messageLogger *messageLogger::mTheOneLogger = nullptr ;

messageLogger::messageLogger()
{
    //Initialize maps
    mCurrentMessage = "";
	mInMessage = false ;
	mSubsystemsEnabled = 0 ;
}

messageLogger &messageLogger::get()
{
    if (mTheOneLogger == nullptr)
		mTheOneLogger = new messageLogger() ;

    return *mTheOneLogger ;
}

void messageLogger::enableType(const messageType &type)
{
    mEnabledModes.push_back(type);
}

void messageLogger::disableType(const messageType &type)
{
    auto it = std::find(mEnabledModes.begin(), mEnabledModes.end(), type) ;
    if (it != mEnabledModes.end())
		mEnabledModes.erase(it) ;
}

void messageLogger::enableSubsystem(uint64_t sys)
{
    mSubsystemsEnabled |= sys ;
}

void messageLogger::disableSubsystem(uint64_t sys)
{
    mSubsystemsEnabled &= ~sys ;
}

bool messageLogger::isMessageTypeEnabled(const messageType &type)
{
    auto it = std::find(mEnabledModes.begin(), mEnabledModes.end(), type) ;
    return it != mEnabledModes.end() ;
}

bool messageLogger::isSubsystemEnabled(uint64_t subs)
{
    if (subs == 0)
		return true ;
    
    return (mSubsystemsEnabled & subs) != 0 ;
}

void messageLogger::startMessage(const messageType &type)
{
    if (mInMessage)
	{
		mCurrentMessage += "FAILED TO CALL ENDMESSAGE" ;
		endMessage() ;
	}
    mCurrentType = type;
    mInMessage = true;
    mCurrentSubsystem = 0 ;
}

void messageLogger::startMessage(const messageType &type, uint64_t sub)
{
    if (mInMessage)
	{
		mCurrentMessage += "FAILED TO CALL ENDMESSAGE - " ;
		endMessage() ;
	}

    mCurrentType = type;
    mInMessage = true;
    mCurrentSubsystem = sub ;
}

void messageLogger::endMessage()
{
    mInMessage = false;
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
    {
		for(auto dest_p : m_destinations)
			dest_p->displayMessage(mCurrentType, mCurrentSubsystem, mCurrentMessage) ;
    }
    mCurrentMessage = "";
}

messageLogger& messageLogger::operator<<(const std::string &value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
		mCurrentMessage.append(value);
    return *this;
}

messageLogger& messageLogger::operator<<(const char *value_p)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
		mCurrentMessage.append(std::string(value_p));
    return *this;
}

messageLogger& messageLogger::operator<<(int value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
		mCurrentMessage.append(std::to_string(value));
    return *this;
}

messageLogger& messageLogger::operator<<(size_t value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
		mCurrentMessage.append(std::to_string(value));
    return *this;
}


messageLogger& messageLogger::operator<<(double value)
{
    if (isMessageTypeEnabled(mCurrentType) && isSubsystemEnabled(mCurrentSubsystem))
		mCurrentMessage.append(std::to_string(value));
    return *this;
}

#ifdef MESSAGE_LOGGER_TEST

void test_logger()
{
    messageLogger logger;

    //Test enable function
    logger.enable(messageLogger::messageType::debug);
    logger.enable(messageLogger::messageType::warning);
    logger.enable(messageLogger::messageType::error);
    logger.enable(messageLogger::messageType::info);

    assert(logger.isMessageTypeEnabled(messageLogger::messageType::debug));
    assert(logger.isMessageTypeEnabled(messageLogger::messageType::warning));
    assert(logger.isMessageTypeEnabled(messageLogger::messageType::info));
    assert(logger.isMessageTypeEnabled(messageLogger::messageType::error));

    logger.disable(messageLogger::messageType::info) ;
    logger.disable(messageLogger::messageType::info) ;

    std::stringstream ss;

    logger.startMessage(messageLogger::messageType::info);
    logger << "This is an info message";
    logger.endMessage(ss);
    assert(ss.str() == "This is an info message");
	
    ss = std::stringstream();;

    logger.startMessage(messageLogger::messageType::warning);
    logger << "This is a warning message";
    logger << ", with two calls" << ", and more calls";
    logger.endMessage(ss);
    assert(ss.str() == "This is a warning message, with two calls, and more calls");

    ss = std::stringstream();;
    logger.startMessage(messageLogger::messageType::info);
    logger << "The left motor is at " << 1.0 << " volts";
    logger << ", and the right motor is at " << 0.8 << " volts";
    logger.endMessage(ss);
    assert(ss.str() == "The left motor is at 1.000000 volts, and the right motor is at 0.800000 volts");
}

int main()
{
    test_logger();
    return 0;
}
#endif // MESSAGE_LOGGER_TEST
