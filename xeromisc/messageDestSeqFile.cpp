#include "messageDestSeqFile.h"
#include <iostream>

namespace xero
{
namespace misc
{

messageDestSeqFile::messageDestSeqFile(const std::string &dir, const std::string &name, unsigned long int timeout)
{
	m_filename = name;
	m_dirname = dir;
	m_enabled = true;
	m_is_open = false;
	m_ref_established = false;
	m_strm_p = nullptr;
	m_timeout_limit = timeout;
	openfile();
}

messageDestSeqFile::~messageDestSeqFile()
{
	if (m_strm_p != nullptr)
		delete m_strm_p;
}

std::string messageDestSeqFile::getFileName(DIR *dir_p)
{
	int index = 0;
	struct dirent *dirent_p;

	while ((dirent_p = readdir(dir_p)) != NULL)
	{
		std::string entname(dirent_p->d_name);
		if (entname.length() > m_filename.length() && entname.substr(0, m_filename.length()) == m_filename)
		{
			size_t endidx;
			std::string endpart = entname.substr(m_filename.length());
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
	std::string ret = m_filename + std::to_string(index);
	std::cout << "returned filename " << ret << std::endl;
	return ret;
}

bool messageDestSeqFile::openfile()
{
	bool ret = false;
	DIR *dir_p;

	//
	// Establish a time base to know when to give up
	//
	if (!m_ref_established)
	{
		start = std::chrono::steady_clock::now();
		m_ref_established = true;
	}

	dir_p = opendir(m_dirname.c_str());
	if (dir_p != NULL)
	{
		//
		// The directory opened, so the flash drive must be mounted now
		//
		std::string filename = getFileName(dir_p);
		closedir(dir_p);

		filename = m_dirname + filename;
		m_strm_p = new std::ofstream(filename);
		if (!m_strm_p->is_open())
		{
			std::cout << "Open of log file '" << filename << "' failed" << std::endl;
			delete m_strm_p;
			m_strm_p = nullptr;
		}
		else
		{
			ret = true;
		}
	}

	return ret;
}

void messageDestSeqFile::displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg)
{
	if (!m_enabled)
		return;

	std::string prefix;
	if (type == messageLogger::messageType::warning)
		prefix == "WARNING: ";

	if (type == messageLogger::messageType::error)
		prefix == "ERROR: ";

	std::string appended_msg = prefix + msg;
	if (m_strm_p != nullptr)
	{
		(*m_strm_p) << (appended_msg) << std::endl;
	}
	else
	{
		if (openfile())
		{
			std::cout << "Succeeded in opening log file." << std::endl;
			for (auto const &m : m_msg_q)
			{
				(*m_strm_p) << m << std::endl;
			}
			m_msg_q.clear();
			(*m_strm_p) << (appended_msg) << std::endl;
		}
		else
		{
			m_msg_q.push_back(appended_msg);
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
			if (elapsed > m_timeout_limit)
			{
				std::cout << "Logging disabled. Timeout reached and log file still not successfully opened." << std::endl;
				m_enabled = false;
				m_msg_q.clear();
			}
		}
	}
}

} // namespace misc
} // namespace xero
