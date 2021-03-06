#include "message_dest_file.h"

void messageDestFile::initialize()
{
	m_strm_p = nullptr;
	setTimeout(15*1000);  // Default to 15 seconds.  Stored in ms.
}

messageDestFile::messageDestFile() {
	initialize();
}

messageDestFile::messageDestFile(const std::string &name)
{
	initialize();
	setFile(name);
}

messageDestFile::~messageDestFile()
{
	if (m_strm_p != nullptr)
		delete m_strm_p;
}

void messageDestFile::setTimeout(unsigned long int timeout_limit) {
	m_timeout_limit = timeout_limit;
}

bool messageDestFile::setFile(const std::string &name)
{
	m_filename = name;
	m_strm_p = new std::ofstream(name);
	if (!m_strm_p->is_open())
	{
		if (!m_ref_established) {
			start = std::chrono::steady_clock::now();
			m_ref_established = true;
    		std::cout << "Warning: Could not open log file '" << name << "' for writing. Will keep trying to reopen it." << std::endl;
		}
		m_strm_p = nullptr;
		return false;
	}
	return true;
}

void messageDestFile::displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg)
{
	if (!m_enabled) {
		return;
	}

	std::string prefix;
	if (type == messageLogger::messageType::warning) prefix == "WARNING: ";
	if (type == messageLogger::messageType::error) prefix == "ERROR: ";
	std::string appended_msg = prefix + msg;
	if (m_strm_p != nullptr) {
		(*m_strm_p) << (appended_msg) << std::endl;
	}
	else {
		if (setFile(m_filename)) {   // Succeeded in opening file
		    std::cout << "Succeeded in opening log file." << std::endl;
			for (auto const& m : m_msg_q) {
				(*m_strm_p) << m << std::endl;
			}
			m_msg_q.clear();
			(*m_strm_p) << (appended_msg) << std::endl;
		}
		else {
			m_msg_q.push_back(appended_msg);
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
			if (elapsed > m_timeout_limit) {
				std::cout << "Logging disabled. Timeout reached and log file still not successfully opened." << std::endl;
				m_enabled = false;
			}
		};
	}
}


#ifdef MESSAGE_DEST_FILE_TEST

#include "message_logger.cpp"

void test1_no_timeout()
{
	auto dest_p = std::make_shared<messageDestFile>("../builddir/non_existent_dir/test_output.log");
	dest_p->setTimeout(1000);  // 1 sec
	messageLogger &logger = messageLogger::get();
	logger.addDestination(dest_p);
	logger.enableType(messageLogger::messageType::debug);

	logger.startMessage(messageLogger::messageType::debug, 0);
	logger << "Test message 1-1";
	logger.endMessage();

	logger.startMessage(messageLogger::messageType::debug, 0);
	logger << "Test message 1-2";
	logger.endMessage();

	// Now create directory so file opening successful
	system("mkdir ../builddir/non_existent_dir");

	logger.startMessage(messageLogger::messageType::debug, 0);
	logger << "Test message 1-3";
	logger.endMessage();
}

void test2_timeout()
{
	auto dest_p = std::make_shared<messageDestFile>("../builddir/non_existent_dir2/test_output.log");
	dest_p->setTimeout(1000);  // 1 sec
	messageLogger &logger = messageLogger::get();
	logger.addDestination(dest_p);
	logger.enableType(messageLogger::messageType::debug);

	logger.startMessage(messageLogger::messageType::debug, 0);
	logger << "Test message 2-1";
	logger.endMessage();

	logger.startMessage(messageLogger::messageType::debug, 0);
	logger << "Test message 2-2";
	logger.endMessage();

    // Wait 2 sec so timeout exceeded
	system("sleep 2");

	logger.startMessage(messageLogger::messageType::debug, 0);
	logger << "Test message 2-3";
	logger.endMessage();
}

int main()
{
	test1_no_timeout();
	test2_timeout();

	return 0;
}

#endif
