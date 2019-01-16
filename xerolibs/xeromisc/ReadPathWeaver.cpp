#include "ReadPathWeaver.h"
#include <sstream>
#include <fstream>
#include <cctype>

namespace xero {
namespace misc {

const std::string ReadPathWeaver::var_prefix_("var:") ;

ReadPathWeaver::ReadPathWeaver(MessageLogger &logger, uint64_t msggroup) : logger_(logger) {
    msggroup_ = msggroup;
}

ReadPathWeaver::~ReadPathWeaver() {
    
}

bool ReadPathWeaver::readLine(const std::string &line, std::vector<double> &values) {
    double value ;
    bool success ;
    std::string buffer ;

    for(unsigned i = 0; i < line.length(); i++) {
        // Check for comment start
        if(line[i] == ',') {
            success = parseDouble(buffer, value);

            if(success == true){
                values.push_back(value) ;
            }
            else {
                return false ;
            }
        } 
        else {
            buffer += line[i];
        }
    }

    success = parseDouble(buffer, value);

    if(success == true){
        values.push_back(value) ;
    }
    else {
        return false ;
    }

    return true;
}

bool ReadPathWeaver::readFile(const std::string &filename) {
    std::ifstream file(filename);
    if(file.bad() || file.fail()) {
        return false;
    }


    std::string line;
    while(std::getline(file, line)) {
        std::vector<double> values ;


        if(readLine(line,  values) && values.size() == 8) {
            path_values segment;
            //value 0 is time
            segment.x = values.at(1);
            segment.y = values.at(2);
            segment.position = values.at(3);
            segment.velocity = values.at(4);
            segment.acceleration = values.at(5);
            segment.jerk = values.at(6);
            segment.heading = values.at(7);

            segments_.push_back(segment);

        }
    }
    return true;
}

bool ReadPathWeaver::parseDouble(const std::string &value, double &result) {
    size_t output;

    try {
        result = std::stod(value, &output);

        // Check for extra characters in value
        if(output != value.length())
            return false;
    } catch(...) {
        return false;
    }

    return true;
}



} // namespace misc
} // namespace xero
