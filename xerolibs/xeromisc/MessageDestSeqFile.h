#pragma once

#include "MessageLoggerDest.h"
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <dirent.h>

/// \file

namespace xero
{
namespace misc
{

/// \brief A sequential file destination for a message logger
///
/// Attempts to open files using the given filename and an increasing index to create unique filenames
class MessageDestSeqFile : public MessageLoggerDest
{
public:
    /// \brief create a new message logger sequential file destination using the given directory, filename, and timeout
    /// \param dir the destination file directory
    /// \param filename the destination file filename
    /// \param timeout the amount of time in milliseconds after which to stop trying to open the file
    MessageDestSeqFile(const std::string &dir, const std::string &filename, unsigned long int timeout = 15000);

    /// \brief destroy the message logger sequential file destination
    virtual ~MessageDestSeqFile();
    
    /// \brief write the given message to the file
    /// \param type the type of the message
    /// \param subs the subsystems the message belongs to
    /// \param msg the message to write
    virtual void displayMessage(const MessageLogger::MessageType &type, uint64_t subs, const std::string &msg);

private:
    bool openfile();
    std::string getFileName(DIR *dir_p);

private:
    std::ofstream *strm_p_;
    std::list<std::string> msg_q_;
    std::string filename_;
    std::string dirname_;
    bool enabled_;
    bool is_open_;
    bool ref_established_;
    std::chrono::time_point<std::chrono::steady_clock> start_;
    int timeout_limit_;
};

} // namespace misc
} // namespace xero
