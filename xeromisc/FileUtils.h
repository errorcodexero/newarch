#pragma once

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

namespace xero {

    namespace file {

        inline bool exists(const std::string& name) {
            struct stat buffer;   
            return (stat (name.c_str(), &buffer) == 0); 
        }
    
        inline bool is_readable(const std::string& name) {
            std::ifstream f(name.c_str());
            return f.good();
        }

    }
    
}
