#pragma once

#include "MessageDestination.h"
#include <fstream>

namespace xerolib
{
	class MessageStreamDestination : public MessageDestination
	{
	public:
		MessageStreamDestination(const char *name_p);
		MessageStreamDestination(std::ostream &strm);
		virtual ~MessageStreamDestination();

		virtual void SendMessage(MessageLogger::MessageType type, const std::string &msg)
		{
			(*m_stream_p) << msg << std::endl << std::flush;
		}

	private:
		std::ostream *m_stream_p;
		bool m_delete_object;;
	};
}


