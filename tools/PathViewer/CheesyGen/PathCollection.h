#pragma once

#include "PathGroup.h"
#include "RobotParams.h"
#include <map>
#include <string>
#include <memory>

namespace xero
{
	namespace paths
	{
		class PathCollection
		{
		public:
			PathCollection() {
			}

			virtual ~PathCollection() {
			}

			size_t getPathCount() const {
				size_t ret = 0;

				for (auto gr : groups_)
					ret += gr.second->getPathCount();

				return ret;
			}

			std::shared_ptr<PathGroup> findGroupByName(const std::string& group) {
				auto it = groups_.find(group);
				if (it == groups_.end())
					return nullptr;

				return it->second;
			}

			std::shared_ptr<RobotPath> findPathByName(const std::string &group, const std::string& name) {
				auto gptr = findGroupByName(group);
				if (gptr == nullptr)
					return nullptr;

				return gptr->findPathByName(name);
			}

			bool hasGroup(const std::string & name) const {
				auto it = groups_.find(name);
				return it != groups_.end();
			}

			std::shared_ptr<PathGroup> addGroup(const std::string& name) {
				std::shared_ptr<PathGroup> gptr = findGroupByName(name);
				if (gptr != nullptr)
					return gptr;

				gptr = std::make_shared<PathGroup>(name);
				groups_[name] = gptr;
				return gptr;
			}

			void addPath(const std::string &group, std::shared_ptr<RobotPath> path) {
				auto gptr = addGroup(group);
				gptr->addPath(path);
			}

			std::vector<std::string> getGroupNames() {
				std::vector<std::string> names;

				for (auto item : groups_)
					names.push_back(item.first);

				return names;
			}

			RobotParams& getRobot() {
				return robot_;
			}

		private:
			std::map<std::string, std::shared_ptr<PathGroup>> groups_;
			RobotParams robot_;
		};
	}
}


