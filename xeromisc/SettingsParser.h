#pragma once

#include <map>
#include "Setting.h"
#include "MessageLogger.h"


/// \file

namespace xero {
	namespace misc {
		/// \brief this class parses a text file for a set of name value pairs
		class SettingsParser {
		public:
			/// \brief create a new settings parser object
			/// \param logger the message logger for logging parse errors
			/// \param msggroup the message group for logging parser errors
			SettingsParser(MessageLogger &logger, uint64_t msggroup);

			/// \brief destroy a settings parser object
			virtual ~SettingsParser();

			/// \brief read a file of values
			///
			/// A value is stored on a single line.  The line consists of a name followed by white space
			/// followed by a value.  A '#' character indicates a comment and all characters including and
			/// following the '#' character are ignored.  The values can be boolean, integer, double, or string.
			/// Strings are delimited with double quotes.
			/// \param filename the name of the file to read
			/// \returns true if the file was read successfully, false is an error occurred
			bool readFile(const std::string &filename);

			/// \brief this method returns true if a value with the given name is present
			/// \param key the name of the value of interest
			/// \returns true if a value with the given name is found
			bool isDefined(const std::string &key) const;

			/// \brief returns true if a value with the given name is present; logs an error otherwise
			/// \param key the name of the value of interest
			/// \param type the type of value being gotten
			/// \returns true if a value with the given name is found
			bool isDefinedOnGet(const std::string &key, const std::string &type) const;

			/// \brief returns the setting object
			/// \param key the name of the value of interest
			/// \returns the setting object
			Setting &get(const std::string &key) ;

			/// \brief associate the given key with the given boolean value
			/// \param key the name to associate with the value
			/// \param value the boolean value to be associated
			void set(const std::string &key, bool value);

			/// \brief associate the given key with the given integer value
			/// \param key the name to associate with the value
			/// \param value the integer value to be associated
			void set(const std::string &key, int value);

			/// \brief associate the given key with the given double value
			/// \param key the name to associate with the value
			/// \param value the double value to be associated
			void set(const std::string &key, double value);

			/// \brief associate the given key with the given string value
			/// \param key the name to associate with the value
			/// \param value the string value to be associated
			void set(const std::string &key, const std::string &value);

			/// \brief get the boolean value associated with the given key; assert if there is no such value
			/// \param key the key for which to get the associated value
			/// \returns the boolean value associated with the given key
			bool getBoolean(const std::string &key) const;

			/// \brief get the boolean value associated with the given key, or a default if there is no such value
			/// \param key the key for which to get the associated value
			/// \param default_value the value to return if there is no value associated with the given key
			/// \returns the boolean value associated with the given key, or the default if none exists
			bool getBoolean(const std::string &key, const bool &default_value) const;

			/// \brief get the integer value associated with the given key; assert if there is no such value
			/// \param key the key for which to get the associated value
			/// \returns the integer value associated with the given key
			int getInteger(const std::string &key) const;

			/// \brief get the integer value associated with the given key, or a default if there is no such value
			/// \param key the key for which to get the associated value
			/// \param default_value the value to return if there is no value associated with the given key
			/// \returns the integer value associated with the given key, or the default if none exists
			int getInteger(const std::string &key, const int &default_value) const;

			/// \brief get the double value associated with the given key; assert if there is no such value
			/// \param key the key for which to get the associated value
			/// \returns the double value associated with the given key
			double getDouble(const std::string &key) const;

			/// \brief get the double value associated with the given key, or a default if there is no such value
			/// \param key the key for which to get the associated value
			/// \param default_value the value to return if there is no value associated with the given key
			/// \returns the double value associated with the given key, or the default if none exists
			double getDouble(const std::string &key, const double &default_value) const;

			/// \brief get the string value associated with the given key; assert if there is no such value
			/// \param key the key for which to get the associated value
			/// \returns the string value associated with the given key
			const std::string &getString(const std::string &key) const;

			/// \brief get the string value associated with the given key, or a default if there is no such value
			/// \param key the key for which to get the associated value
			/// \param default_value the value to return if there is no value associated with the given key
			/// \returns the string value associated with the given key, or the default if none exists
			const std::string &getString(const std::string &key, const std::string &default_value) const;

		private:
			const Setting &getSetting(const std::string &key, const std::string &type) const;
			const Setting &getSetting(const std::string &key, const Setting &default_value, const std::string &type) const;

			bool parseBoolean(const std::string &value, bool &result);
			bool parseInteger(const std::string &value, int &result);
			bool parseDouble(const std::string &value, double &result);
			bool parseString(const std::string &value, std::string &result);

			bool readLine(const std::string &line, std::string &key, std::string &value, bool &is_string, const std::string &filename, int line_num);

		private:
			static const std::string var_prefix_ ;
			
			MessageLogger &logger_;
			uint64_t msggroup_;
			
			std::map<std::string, Setting> settings_;
		};
	}
}
