#pragma once

#include <string>
#include <algorithm>


/// \file

namespace xero {

    namespace string {

        /// \brief return a lower case version of the string
        /// \param s the string to convert
        /// \returns a lower case version of the string
        inline std::string downcase(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }
    
        /// \brief return an upper case version of the string
        /// \param s the string to convert
        /// \returns an upper case version of the string    
        inline std::string upcase(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
            return s;
        }

        /// \brief returns true if the string ends with a given value
        /// \param main the string to check
        /// \param ending the string to check for at the end
        /// \returns true if the string ends with the value given
        inline bool endsWith(const std::string& main, const std::string& ending) {
            if (main.size() >= ending.size() &&
                main.compare(main.size() - ending.size(), ending.size(), ending) == 0) {
                return true;
            } else {
                return false;
            }
        }

        /// \brief returns true if the string starts with a given value
        /// \param main the string to check
        /// \param ending the string to check for at the start
        /// \returns true if the string starts with the value given
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
