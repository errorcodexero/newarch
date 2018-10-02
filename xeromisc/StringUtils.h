#pragma once

#include <string>
#include <algorithm>

namespace xero {

    namespace string {

        inline std::string downcase(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }
    
        inline std::string upcase(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
            return s;
        }

        inline bool endsWith(const std::string& main, const std::string& ending) {
            if (main.size() >= ending.size() &&
                main.compare(main.size() - ending.size(), ending.size(), ending) == 0) {
                return true;
            } else {
                return false;
            }
        }

        inline bool startsWith(const std::string& main, const std::string &start) {
            if (main.size() >= start.size() &&
                main.compare(0, start.size(), start) == 0) {
                return true;
            } else {
                return false;
            }
        }

    }
        
}
