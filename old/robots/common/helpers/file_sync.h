#pragma once
#include "sync.h"


class fileSync :public sync {
private:
	std::string currentMessage;
public:
	fileSync();
	void getMessage(const std::string&);
	void getMessage(const std::vector<std::string>&);
	void sendMessage(std::ofstream&);
};