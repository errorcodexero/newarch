#pragma once

#include <map>
#include "Setting.h"
#include "messageLogger.h"

namespace xero {
	namespace misc {
		/// \brief this class parses a text file for a set of name value pairs
		class SettingsParser {
		public:
			/// \brief create a new settings parser object
			/// \param logger the message logger for logging parse errors
			/// \param msggroup the message group for logging parser errors
			SettingsParser(messageLogger &logger, uint64_t msggroup);

			/// \brief destroy a settings parser object
			virtual ~SettingsParser();

			/// \brief read a file of values.
			/// A value is stored on a single line.  The line consists of a name followed by white space
			/// followed by a value.  A '#' character indicates a comment and all characters including and
			/// following the '#' character are ignored.  The values can be integer, boolean, real, or string.
			/// Strings are delimited with double quotes.
			/// \param filename the name of the file to read
			/// \returns true if the file was read successfully, false is an error occurred
			bool readFile(const std::string &filename);

			/// \brief this method returns true if a value with the given name is present
			/// \param key the name of the value of interest
			/// \returns true if a value with the given name is found
			bool isDefined(const std::string &key);

			void set(const std::string &key, bool value);
			void set(const std::string &key, int value);
			void set(const std::string &key, double value);
			void set(const std::string &key, const std::string &value);

			bool getBoolean(const std::string &key);
			int getInteger(const std::string &key);
			double getDouble(const std::string &key);
			std::string getString(const std::string &key);

		private:
			bool parseBoolean(const std::string &value, bool &result);
			bool parseInteger(const std::string &value, int &result);
			bool parseDouble(const std::string &value, double &result);
			bool parseString(const std::string &value, std::string &result);

			bool readLine(const std::string &line, std::string &key, std::string &value, bool &is_string, const std::string &filename, int line_num);

			messageLogger &logger_;
			uint64_t msggroup_;
			
			std::map<std::string, Setting> settings_;
		};
	}
}
