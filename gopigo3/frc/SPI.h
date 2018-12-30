#pragma once

#include "ErrorBase.h"
#include <cstring>

namespace frc
{
	//
	// This is just an empty shell meant to provide enough of a class to compile the 
	// NavX software.  It is currently not functionaly and does not access the SPI port
	// on the raspberry PI (which is already devoted to the GoPiGo3 robot).
	//
	class SPI
	{
	public:
		enum class Port
		{
			Port_0 = 0,
			Port_1 = 1,
		};

	public:
		SPI(Port SPIport)
		{
			m_port = SPIport;
		}

		virtual ~SPI()
		{
		}
	
		virtual int Read(bool initiate, uint8_t *dataReceived, int size)
		{
			return size;
		}

		virtual int Write(uint8_t *data, int size)
		{
			return size;
		}

		void SetChipSelectActiveLow()
		{
		}

		void SetClockActiveLow()
		{
		}

		void SetClockRate(double hz)
		{
		}

		void SetMSBFirst()
		{
		}

		void SetSampleDataOnFalling()
		{
		}

		void SetSampleDataOnTrailingEdge()
		{
		}

	private:
		Port m_port;
	};
}

