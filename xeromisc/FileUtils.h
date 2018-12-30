#pragma once

/// \file

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

namespace xero {

    namespace file {

        /// \brief Checks if the file exists.
        /// \param Name of the file.
        /// \returns true if the file exists.
        inline bool exists(const std::string& name) {
            struct stat buffer;   
            return (stat(name.c_str(), &buffer) == 0); 
        }
    
        /// \brief Checks if the file is readable.
        /// \param Name of the file.
        /// \returns true if the file is readable.
        inline bool is_readable(const std::string& name) {
            std::ifstream f(name.c_str());
            return f.good();
        }

        /// \brief Checks if the file exists and is a symlink.
        /// \param Name of the file.
        /// \returns true if the file exists and is a symlink.
        inline bool is_a_symlink(const std::string& name) {
            struct stat buffer;   
            return (lstat(name.c_str(), &buffer) == 0 && S_ISLNK(buffer.st_mode)); 
        }


        /// \brief Create a symlink from a filename to another.
        ///        Only overwrite an existing file if it's already a symlink.
        /// \returns true upon success, false upon failure.
        inline bool create_symlink(const std::string& dest, const std::string& link) {
            // Ensure the destination file exists
            assert(exists(dest));

            // If link exists and is already a symlink, delete it so we may create a new link
            if (exists(link)) {
                if (!is_a_symlink(link)) {
                    // Existing file not a symlink so don't try to delete
                    return false;
                } else if (unlink(link.c_str()) != 0) {   
                    // Deleting existing symlink was NOT successful
                    return false;
                }
            }

            // Create the symlink
            int ret = symlink(dest.c_str(), link.c_str());
            return (ret == 0);
        }

    }
    
}
