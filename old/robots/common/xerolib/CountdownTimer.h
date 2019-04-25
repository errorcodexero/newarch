#pragma once

namespace xero
{
	namespace base
	{
		class CountdownTimer
		{
		public:
			CountdownTimer()
			{
			}

			virtual ~CountdownTimer()
			{
			}

			void set(double now, double length)
			{
				m_start_time = now;
				m_timer_length = length;
			}

			bool done(double now)
			{
				return now - m_start_time > m_timer_length;
			}

		private:
			double m_start_time;
			double m_timer_length;
		};
	}
}