#pragma once

/// \file

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

namespace xero {

    namespace file {

        /// \brief Returns true if the file exists
        /// \param name of the file
        /// \returns true if the file exists
        inline bool exists(const std::string& name) {
            struct stat buffer;   
            return (stat (name.c_str(), &buffer) == 0); 
        }
    
        /// \brief Returns true if the file is readable
        /// \param name of the file
        /// \returns true if the file is readable
        inline bool is_readable(const std::string& name) {
            std::ifstream f(name.c_str());
            return f.good();
        }

    }
    
}
