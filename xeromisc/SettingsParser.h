#pragma once

#include <map>
#include "Setting.h"
#include "messageLogger.h"

namespace xero {
	namespace misc {
		class SettingsParser {
		public:
			SettingsParser(messageLogger &logger, uint64_t msggroup);
			~SettingsParser();
			
			bool readFile(const std::string &filename);

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
