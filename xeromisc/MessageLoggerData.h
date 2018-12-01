#pragma once

#include <string>
#include <map>

namespace xero {
	namespace misc {

		/// \brief this class represents data to be logged to the message logger
		class MessageLoggerData {
		public:
			/// \brief create a new logger data entry
			MessageLoggerData();

			/// \brief create a new logger data entry with the given label
			/// \param label the label for the data
			MessageLoggerData(const std::string &label);

			/// \brief initialize a logger data entry with the label given
			/// \param label the label for the data
			/// \returns the MessageLoggerData object reference
			MessageLoggerData &init(const std::string &label);

			/// \brief add data to the data logger entry
			/// \param key the name of the data
			/// \param value the value for the data
			/// \returns the MessageLoggerData object reference
			MessageLoggerData &add(const std::string &key, const std::string &value);

			/// \brief add data to the data logger entry
			/// \param key the name of the data
			/// \param value the value for the data
			/// \returns the MessageLoggerData object reference
			MessageLoggerData &add(const std::string &key, const char *value);

			/// \brief add data to the data logger entry
			/// \param key the name of the data
			/// \param value the value for the data
			/// \returns the MessageLoggerData object reference
			MessageLoggerData &add(const std::string &key, double value);

			/// \brief add data to the data logger entry
			/// \param key the name of the data
			/// \param value the value for the data
			/// \returns the MessageLoggerData object reference
			MessageLoggerData &add(const std::string &key, int value);

			/// \brief add data to the data logger entry
			/// \param key the name of the data
			/// \param value the value for the data
			/// \returns the MessageLoggerData object reference
			MessageLoggerData &add(const std::string &key, size_t value);

			/// \brief add data to the data logger entry
			/// \param key the name of the data
			/// \param value the value for the data
			/// \returns the MessageLoggerData object reference
			MessageLoggerData &add(const std::string &key, bool value);

			/// \brief convert to a string
			/// \returns a string representing the data stored
			std::string toString();

		private:
			std::string label_;
			std::map<std::string, std::string> data_;
		};

	} // namespace misc
} // namespace xero
