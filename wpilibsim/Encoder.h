#pragma once

#include <cstdint>

namespace frc
{
	class Encoder
	{
	public:
		Encoder(int first, int second);
		virtual ~Encoder();

		void Reset();
		int32_t Get();

	private:
		int m_first;
		int m_second;
	};
}

