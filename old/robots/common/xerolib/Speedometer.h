#pragma once

#include <vector>
#include <cassert>
#include <mutex>

namespace xero
{
	namespace base
	{
		class Speedometer
		{
		public:
			/// \brief create a speedometer
			/// \param samples the number of position samples to keep to calculate speed
			Speedometer(size_t samples = 2)
			{
				m_next = 0;
				m_full = false;
				m_samples.resize(samples);
			}

			/// \brief destroy the speedometer
			virtual  ~Speedometer()
			{
			}

			/// \brief set the number of samples averaged for speed
			/// Note this method resets the contents of the speedometer
			void setSampleCount(size_t count)
			{
				m_next = 0;
				m_full = false;
				m_samples.resize(count);
			}

			/// \brief reset with zero samples
			void reset()
			{
				m_full = false;
				m_next = 0;
			}

			/// \brief add a new sample
			/// If we have reached the maximum number of samples, drop the oldest sample
			/// \param time the time for this sample
			/// \param distance the distance for this sample
			void addSample(double time, double distance)
			{
				TimePoint pt(time, distance);

				m_samples[m_next++] = pt;
				if (m_next == m_samples.size())
				{
					m_full = true;
					m_next = 0;
				}
			}

			/// \brief return true if a value speed can be determined
			/// A valid speed cannot be determined unless there have been
			/// at lead two sample added.  If less than two samples have been
			/// added this method returns false
			/// \returns true if a speed can be calculated, otherwise false
			bool isValid() const
			{
				return isValidInternal();
			}

			/// \brief returns the speed based on the samples stored
			/// \returns the calculated speed
			double getSpeed() const
			{
				if (!isValidInternal())
					return 0.0;

				const TimePoint &newest = getNewestSample();
				const TimePoint &oldest = getOldestSample();

				return (newest.distance - oldest.distance) / (newest.time - oldest.time);
			}

			double getLastSample() const
			{
				const TimePoint &newest = getNewestSample();
				return newest.distance;
			}

		private:
			bool isValidInternal() const
			{
				return m_full || (m_next > 1);

			}

		private:
			struct TimePoint
			{
				TimePoint()
				{
					time = 0;
					distance = 0;
				}

				TimePoint(double t, double d)
				{
					time = t;
					distance = d;
				}
				double time;
				double distance;
			};

		private:
			const TimePoint &getOldestSample() const
			{
				if (m_full)
					return m_samples[m_next];

				return m_samples[0];
			}

			const TimePoint &getNewestSample() const
			{
				size_t index;

				if (m_next == 0)
					index = m_samples.size() - 1;
				else
					index = m_next - 1;

				return m_samples[index];
			}

		private:
			size_t m_next;
			bool m_full;

			//
			// Samples
			//
			std::vector<TimePoint> m_samples;
		};
	}
}