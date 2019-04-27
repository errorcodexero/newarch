#pragma once

#include "RobotPath.h"
#include <map>
#include <string>
#include <memory>

namespace xero
{
	namespace paths
	{
		class PathGroup
		{
		public:
			PathGroup(const std::string& name) {
				name_ = name;

			}

			virtual ~PathGroup() {
			}

			const std::string& getName() const {
				return name_;
			}

			std::shared_ptr<RobotPath> findPathByName(const std::string& name) {
				auto it = paths_.find(name);
				if (it == paths_.end())
					return nullptr;

				return it->second;
			}

			bool hasPath(const std::string& name) const {
				auto it = paths_.find(name);
				return it != paths_.end();
			}

			void addPath(std::shared_ptr<RobotPath> path) {
				paths_[path->getName()] = path;
			}

			std::vector<std::string> getPathNames() {
				std::vector<std::string> names;

				for (auto item : paths_)
					names.push_back(item.first);

				return names;
			}

		private:
			std::string name_;
			std::map<std::string, std::shared_ptr<RobotPath>> paths_;
		};
	}
}

