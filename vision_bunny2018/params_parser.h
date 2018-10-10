#pragma once
#include <string>
#include <fstream>
#include <map>
#include <cassert>
#include <optional>


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
    bool hasParam(const std::string &paramName);

    /// \brief gets the value of a parameter
    /// and returns a default value if the parameter does not exist
    /// \param paramName the name of the parameter we are searching for
    /// \param defaultValue the value to return if the parameter did not exist in the parameter file. If no defaultValue specified and parameter not found, give error.
    /// \return the parameter value from the parameter file, or defaultValue if the parameter did not exist in the file
    double getValue(const std::string &paramName, std::optional<double> defaultValue = std::nullopt);

    /// \brief gets the value of a parameter
    /// and returns a default value if the parameter does not exist
    /// \param paramName the name of the parameter we are searching for
    /// \param defaultValue the value to return if the parameter did not exist in the parameter file
    /// \return the parameter value from the parameter file, or defaultValue if the parameter did not exist in the file
    std::string getString(const std::string& paramName, const std::string& defaultValue);
    std::string getString(const std::string& paramName);

    /// \brief print the parameter values to the output stream given
    /// \param out the output stream to write the parameters to
    bool printMap(std::ostream &out);
    
private:
    // the map that stores the parameter values
    std::map<std::string, std::string> mParamsMap;

    // the name of the file read
    std::string mFilename;
};
