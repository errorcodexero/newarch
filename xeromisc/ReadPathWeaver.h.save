#pragma once

#include <map>
#include "Setting.h"
#include "MessageLogger.h"


/// \file

namespace xero {
    namespace misc {
        /// \brief this class parses a text file for a set of name value pairs
        class ReadPathWeaver {
        public:
            /// \brief create a new settings parser object
            /// \param logger the message logger for logging parse errors
            /// \param msggroup the message group for logging parser errors
            ReadPathWeaver(MessageLogger &logger, uint64_t msggroup);

            /// \brief destroy a settings parser object
            virtual ~ReadPathWeaver();

            /// \brief read a file of values
            ///
            /// A value is stored on a single line.  The line consists of a name followed by white space
            /// followed by a value.  A '#' character indicates a comment and all characters including and
            /// following the '#' character are ignored.  The values can be boolean, integer, double, or string.
            /// Strings are delimited with double quotes.
            /// \param filename the name of the file to read
            /// \returns true if the file was read successfully, false is an error occurred
            bool readFile(const std::string &filename);

        private:
            bool parseDouble(const std::string &value, double &result);
            bool readLine(const std::string &line, std::string &key, std::string &value, bool &is_string, const std::string &filename, int line_num);

            struct path_values
                {
                double x;
                double y;
                double position;
                double velocity;
                double acceleration;
                double jerk;
                double heading;
                } path_values_;


        private:
            static const std::string var_prefix_ ;
            
            MessageLogger &logger_;
            uint64_t msggroup_;
            
            
        };
    }
}
