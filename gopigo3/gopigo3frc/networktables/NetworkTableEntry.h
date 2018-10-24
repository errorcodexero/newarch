#pragma once

namespace nt
{
	//
	// This is a basic emptry class for the network table entries.  It is meant
	// to supply just enough capability to compile the NavX software.  It is non
	// functional.
	//
	class NetworkTableEntry
	{
	public:
		NetworkTableEntry()
		{
		}

		virtual ~NetworkTableEntry()
		{
		}

		explicit operator bool() const
		{
			return true;
		}

		void SetDouble(double d)
		{
		}
	};

	class NetworkTable
	{
	public:
		NetworkTable()
		{
		}

		virtual ~NetworkTable()
		{
		}

		NetworkTableEntry GetEntry(const char *name_p)
		{
			NetworkTableEntry entry;
			return entry;
		}
	};
}
