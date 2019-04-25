#include "MessageStreamDestination.h"

namespace xero
{
	namespace base
	{
		MessageStreamDestination::MessageStreamDestination(const char *name_p)
		{
			m_stream_p = new std::ofstream(name_p);
			m_delete_object = true;
		}

		MessageStreamDestination::MessageStreamDestination(std::ostream &strm)
		{
			m_stream_p = &strm;
			m_delete_object = false;
		}

		MessageStreamDestination::~MessageStreamDestination()
		{
			if (m_delete_object)
				delete m_stream_p;
		}
	}
}