#pragma once
#include "sync.h"


class consoleSync :public sync {
private:
	std::string currentMessage;
public:
	consoleSync();
	void getMessage(const std::string&);
	void getMessage(const std::vector<std::string>&);
	void sendMessage(std::ostream&);
};