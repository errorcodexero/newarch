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
    settings_.clear();
}

bool SettingsParser::readLine(const std::string &line, std::vector<double> &values) {
    double value ;
    bool success ;
    std::stringstream buffer ;

    for(unsigned i = 0; i < line.length(); i++) {
        // Check for comment start
        if(line[i] == ',') {
            success = parseDouble(buffer, value)

            if(success == true){
                values.push_back(value) ;
            }
            else {
                return false ;
            }
        } 
        else {
            buffer.put(line[i]);
        }
    }

    success = parseDouble(buffer, value)

    if(success == true){
        values.push_back(value) ;
    }
    else {
        return false ;
    }

    return true;
}

bool SettingsParser::readFile(const std::string &filename) {
    std::ifstream file(filename);
    if(file.bad() || file.fail()) {
        return false;
    }

    int line_num = 0;

    bool bool_output;
    int int_output;
    double double_output;
    std::string string_output;

    std::string line;
    while(std::getline(file, line)) {
        std::vector<double> values ;
        bool is_string = false;

        line_num++;

        if(readLine(line,  values) && value.length() == 8) {
            path_values segment = {}
        }
    }
    return true;
}

bool SettingsParser::parseDouble(const std::string &value, double &result) {
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
