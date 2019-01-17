#include "params_parser.h"
#include <ostream>
#include <string>
#include <iostream>

paramsInput::paramsInput()
{
}

bool paramsInput::readFile(const std::string &filename)
{
    bool ret = false ;
    this->mFilename = filename;

    std::ifstream f(filename) ;
    if (f.is_open())
    {
        std::string buffer;
    
        while (!f.eof())
        {
            std::getline(f, buffer);
            unsigned int commentStart = buffer.find('#');

            if (commentStart != buffer.npos)
            {
                buffer = buffer.substr(0, commentStart);
            }
      
            if (buffer.length() > 0)
            {
                unsigned int i = 0;
                std::string Key, valueString;
                double value;
                while (i < buffer.length() && (!isspace(buffer[i])))
                    Key += buffer[i++];

                while (i < buffer.length() && (isspace(buffer[i])))
                    i++;

                while (i < buffer.length() && (!isspace(buffer[i])))
                    valueString += buffer[i++];

                mParamsMap.insert(std::pair<std::string, std::string>(Key, valueString));
            }
        }
        ret = true ;
    }
    else
    {
        std::cout << "Could not open file '" << filename << "'" << std::endl ;
    }

    return ret ;
}

bool paramsInput::printMap(std::ostream &fo) const
{
    for (auto it = mParamsMap.begin() ; it != mParamsMap.end(); it++)
    {
        fo << it->first << ' ' << it->second << std::endl;
    }
    
    return true ;
}

bool paramsInput::hasParam(const std::string &paramName) const
{
    bool found = false;
    if (mParamsMap.find(paramName) != mParamsMap.end())
        found = true;
    
    return found;
}

double paramsInput::getValue(const std::string &paramName) const
{
    auto it = mParamsMap.find(paramName) ;
    if (it == mParamsMap.end()) {
        std::cerr << "No parameter called '" << paramName << "' found." << std::endl;
        exit(-1);
    }
    
    return std::stod((it->second));
}

std::string paramsInput::getString(const std::string& paramName) const
{
    auto it = mParamsMap.find(paramName) ;
    if (it == mParamsMap.end()) {
        std::cerr << "No parameter called '" << paramName << "' found." << std::endl;
        exit(-1);
    }
    
    return it->second ;
}

#ifdef  PARAMS_PARSER_TEST
static const char *paramsFile = "paramsTest.txt";



void test_paramsParser(const std::string &filename, const std::map<std::string, double> &stdMap)
{
    paramsInput params;
    assert(params.readFile(filename));
    for (auto it = stdMap.begin(); it != stdMap.end(); it++) {
        assert(params.hasParam(it->first));
        double value = params.getValue(it->first, -9001);
        assert(value != -9001);
        assert(value == it->second);
    }
}

void createStdMap(std::map<std::string, double> &stdMap) {
    stdMap["drivebase:diameter"] = 2.61811;
    stdMap["drivebase:straight:threshold"] = 0.5;
    stdMap["drivebase:straight:switch:threshold"] = 10000;
    stdMap["drivebase:distance:p"] = 0.02;
    stdMap["drivebase:distance:i"] = 0.02;
    stdMap["drivebase:distance:d"] = 0;
    stdMap["drivebase:distance:f"] = 0;
    stdMap["drivebase:distance:imax"] = 25;
    stdMap["drivebase:angleCorrection:p"] = 0.1;
    stdMap["drivebase:angleCorrection:i"] = 0.1;
    stdMap["drivebase:angleCorrection:d"] = 0;
    stdMap["drivebase:angleCorrection:f"] = 0;
    stdMap["drivebase:angleCorrection:imax"] = 10;
    stdMap["drivebase:angle:threshold"] = 1;
    stdMap["drivebase:angle:switch:threshold"] = 100000;
    stdMap["drivebase:angle:p"] = 0;
    stdMap["drivebase:angle:i"] = 0;
    stdMap["drivebase:angle:f"] = 0;
    stdMap["drivebase:angle:imax"] = 1e+20;
    stdMap["drivebase:straight:accel"] = 0;
    stdMap["drivebase:straight:decel"] = 0;
    stdMap["drivebase:straight:maxspeed"] = 0;
    stdMap["drivebase:straight:maxage"] = 0;
    stdMap["drivebase:straight:p"] = 0;
    stdMap["drivebase:straight:i"] = 0;
    stdMap["drivebase:straight:d"] = 0;
    stdMap["drivebase:straight:f"] = 0;
    stdMap["drivebase:straight:imax"] = 1e+20;
    stdMap["drivebase:error:threshold"] = 3;
    stdMap["drivebase:angle:accel"] = 0;
    stdMap["drivebase:angle:decel"] = 0;
    stdMap["drivebase:angle:maxspeed"] = 0;
    stdMap["drivebase:angle:maxage"] = 0;
    stdMap["drivebase:rotational:p"] = 0;
    stdMap["drivebase:rotational:i"] = 0;
    stdMap["drivebase:rotational:d"] = 0;
    stdMap["drivebase:rotational:imax"] = 1e+20;

}
int main() {
    std::map<std::string, double> stdMap;
    createStdMap(stdMap);
    test_paramsParser(paramsFile, stdMap);
}

#endif //  PARAMSPARSER_TEST

