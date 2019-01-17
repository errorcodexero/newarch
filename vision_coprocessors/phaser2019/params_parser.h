#pragma once
#include <string>
#include <fstream>
#include <map>
#include <cassert>


class paramsInput {
    
public:
    /// \brief create a new params parsing object
    paramsInput();

    /// \brief read the input file containing parameters
    /// \param filename the name of the file to read
    bool readFile(const std::string &filename);

    /// \brief returns true if the parameter parsing object has a parameter with the name given
    /// \params paramName the name of the parameter we are searching for
    /// \returns true if the parameter is found, otherwise false
    bool hasParam(const std::string &paramName) const;

    /// \brief gets the value of a parameter, give error if parameter unspecified
    /// \param paramName the name of the parameter we are searching for
    /// \return the parameter value from the parameter file, or defaultValue if the parameter did not exist in the file
    double getValue(const std::string &paramName) const;

    /// \brief gets the value of a parameter, give error if parameter unspecified
    /// \param paramName the name of the parameter we are searching for
    /// \return the parameter value from the parameter file, or defaultValue if the parameter did not exist in the file
    std::string getString(const std::string& paramName) const;

    /// \brief print the parameter values to the output stream given
    /// \param out the output stream to write the parameters to
    bool printMap(std::ostream &out) const;
    
private:
    // the map that stores the parameter values
    std::map<std::string, std::string> mParamsMap;

    // the name of the file read
    std::string mFilename;
};
