#include "SubsystemModel.h"
#include "RobotSimBase.h"
#include <cctype>

namespace xero {
    namespace sim {
        SubsystemModel::SubsystemModel(RobotSimBase &simbase, const std::string &name) : simbase_(simbase) {
            name_ = name ;
        }

        SubsystemModel::~SubsystemModel() {
        }

        std::mutex &SubsystemModel::getLockMutex() {
            RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
            return sim.getLockMutex() ;            
        }

		bool SubsystemModel::parseDouble(const std::string &prop, double &value) {
			size_t index = 0 ;

			try {
				value = std::stod(prop, &index) ;
			}
			catch(const std::invalid_argument &) {
				return false ;
			}
			catch(const std::out_of_range &) {
				return false ;
			}				

			if (index != prop.length())
				return false ;

			return true ;
		}

		bool SubsystemModel::parseDoubleList(const std::string &list, std::vector<double> &values) {
			size_t index = 0 ;
			std::string value ;

			while (index < list.length()) {
				// Skip leading spaces
				while (index < list.length() && std::isspace(list[index]))
					index++ ;

				// Gather characters until space, comma, or end of line
				while (index < list.length()) {
					if (std::isspace(list[index]) || list[index] == ',') {
						break ;
					}
					value += list[index++] ;
				}

				// Skip spaces after the token and before any comma
				while (index < list.length() && std::isspace(list[index]))
					index++ ;				

				// Parse the value and push on vector for values
				size_t off ;
				double v ;
				try {
					v = std::stod(value, &off) ;
				}
				catch(const std::invalid_argument &) {
					return false ;
				}
				catch(const std::out_of_range &) {
					return false ;
				}				
				if (off != value.length())
					return false ;
				values.push_back(v) ;
				value.clear() ;

				//
				// In all cases, we should be at a comma, or at the end of the line
				//
				if (index != list.length()) {
					if (list[index] != ',')
						return false ;

					index++ ;
				}
			}

			return true ;
		}
    }
}