#include <frc/SimEventManager.h>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace xero {
    namespace sim {
        SimEventManager::SimEventManager() {            
        }

        SimEventManager::~SimEventManager() {
        }

        void SimEventManager::split(const std::string &line, std::vector<std::string> &results) {
            results.clear() ;
            std::string word ;

            size_t index = 0 ;
            while(index < line.length()) {
                if (line[index] == ',') {
                    results.push_back(word) ;
                    word.clear() ;
                }
                else {
                    word += line[index] ;
                }
                index++ ;
            }

            if (word.length() > 0)
                results.push_back(word) ;
        }

        void SimEventManager::addEvent(const std::string &model, const std::string &event, double time, int value) {
            SimEvent evobj(model, event, time, value) ;

            events_.push_back(evobj) ;

            std::sort(events_.begin(), events_.end(), [](const SimEvent &a, const SimEvent &b) -> bool
            {
                return a.getTime() < b.getTime() ;
            }) ;
        }

        bool SimEventManager::readEvents(const std::string &filename) {
            std::ifstream in(filename) ;
            std::string line ;
            std::vector<std::string> words ;
            size_t lineno = 0 ;

            if (in.bad() || in.fail())
            {
                std::cerr << "error: could not read file '" << filename << "'" << std::endl;
                return false ;
            }

            while (std::getline(in, line)) {
                lineno++ ;
                size_t index = 0 ;
                while (index < line.length() && std::isspace(line[index]))
                    index++ ;

                if (index == line.length()) {
                    //
                    // Blank line, continue
                    //
                    continue ;
                }

                if (line[index] == '#') {
                    //
                    // Comment line only, continue
                    //
                    continue ;
                }

                index = line.find('#') ;
                if (index != std::string::npos)
                    line = line.substr(0, index - 1) ;

                split(line, words) ;

                if (words.size() != 4)
                    continue ;

                int value ;
                double time ;
                try {
                    time = std::stod(words[0]) ;
                }
                catch(...) {
                    std::cerr << "ReadSimEvents: Invalid time value '" << words[0] << "' in line " << lineno << " - line ignored" << std::endl ;
                    continue ;
                }

                try {
                    value = std::stoi(words[3]) ;
                }
                catch(...) {
                    std::cerr << "ReadSimEvents: Invalid event value '" << words[3] << "' in line " << lineno << " - line ignored" << std::endl ;
                    continue ;                    
                }

                addEvent(words[1], words[2], time, value) ;                
            }

            return true ;
        }
    }
}