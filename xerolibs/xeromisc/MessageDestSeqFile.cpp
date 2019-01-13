#include "MessageDestSeqFile.h"
#include "FileUtils.h"
#include <iostream>

namespace xero
{
namespace misc
{

MessageDestSeqFile::MessageDestSeqFile(const std::string &dir, const std::string &name, unsigned long int timeout)
{
    filename_ = name;
    dirname_ = dir;
    enabled_ = true;
    is_open_ = false;
    ref_established_ = false;
    strm_p_ = nullptr;
    timeout_limit_ = timeout;
    openfile();
}

MessageDestSeqFile::~MessageDestSeqFile()
{
    if (strm_p_ != nullptr)
        delete strm_p_;
}

std::string MessageDestSeqFile::getFileName(DIR *dir_p)
{
    int index = 0;
    struct dirent *dirent_p;

    while ((dirent_p = readdir(dir_p)) != NULL)
    {
        std::string entname(dirent_p->d_name);
        if (entname.length() > filename_.length() && entname.substr(0, filename_.length()) == filename_)
        {
            size_t endidx;
            std::string endpart = entname.substr(filename_.length());
            int num = std::stoi(endpart, &endidx);
            if (endidx != endpart.length())
            {
                continue;
            }

            if (num >= index)
                index = num;
        }
    }

    index++;
    std::string ret = filename_ + std::to_string(index);
    return ret;
}

bool MessageDestSeqFile::openfile()
{
    bool ret = false;
    DIR *dir_p;

    //
    // Establish a time base to know when to give up
    //
    if (!ref_established_)
    {
        start_ = std::chrono::steady_clock::now();
        ref_established_ = true;
    }

    dir_p = opendir(dirname_.c_str());
    if (dir_p != NULL)
    {
        //
        // The directory opened, so the flash drive must be mounted now
        //
        std::string filename = getFileName(dir_p);
        closedir(dir_p);

        filename = dirname_ + filename;
        strm_p_ = new std::ofstream(filename);
        if (!strm_p_->is_open())
        {
            std::cout << "Open of log file '" << filename << "' failed" << std::endl;
            delete strm_p_;
            strm_p_ = nullptr;
        }
        else
        {
            ret = true;
        }

        //
        // For convenience, always create a symlink point to the latest log file
        //
        const std::string latest_file_symlink_name(dirname_ + "/latest");
        xero::file::create_symlink(filename, latest_file_symlink_name);
    }

    return ret;
}

void MessageDestSeqFile::displayMessage(const MessageLogger::MessageType &type, uint64_t subs, const std::string &msg)
{
    if (!enabled_)
        return;

    std::string prefix;
    if (type == MessageLogger::MessageType::warning)
        prefix == "WARNING: ";

    if (type == MessageLogger::MessageType::error)
        prefix == "ERROR: ";

    std::string appended_msg = prefix + msg;
    if (strm_p_ != nullptr)
    {
        (*strm_p_) << (appended_msg) << std::endl;
    }
    else
    {
        if (openfile())
        {
            std::cout << "Succeeded in opening log file." << std::endl;
            for (auto const &m : msg_q_)
            {
                (*strm_p_) << m << std::endl;
            }
            msg_q_.clear();
            (*strm_p_) << (appended_msg) << std::endl;
        }
        else
        {
            msg_q_.push_back(appended_msg);
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_).count();
            if (elapsed > timeout_limit_)
            {
                std::cout << "Logging disabled. Timeout reached and log file still not successfully opened." << std::endl;
                enabled_ = false;
                msg_q_.clear();
            }
        }
    }
}

} // namespace misc
} // namespace xero
