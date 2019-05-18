#pragma once

#include "PathCollection.h"
#include <string>

class JSONValue;

namespace xero {
	namespace paths
	{
		class JSONPathReader
		{
		public:
			JSONPathReader() = delete;
			virtual ~JSONPathReader() = delete;

			static bool readJSONPathFile(const std::string& filename, PathCollection& paths);

		private:
			static std::wstring toWide(const std::string& str);
			static std::string toNarrow(const std::wstring& str);
			static bool readFile(const std::string& filename, std::wstring& data);
			static bool parseRobot(const std::string& filename, PathCollection& paths, JSONValue* robot);
			static bool parseGroups(const std::string& filename, PathCollection& paths, JSONValue* groups);
			static bool parseGroup(const std::string& filename, PathCollection& paths, JSONValue *group);
			static bool parsePath(const std::string& filename, PathCollection& paths, const std::string &path, JSONValue* group);
		};
	}
}

