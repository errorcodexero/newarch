#pragma once

#include "MessageLogger.h"

namespace xerolib
{
	class MessageDestination
	{
	public:
		MessageDestination()
		{
		}

		virtual ~MessageDestination()
		{
		}

		virtual void SendMessage(MessageLogger::MessageType type, const std::string &msg) = 0;
	};
}
