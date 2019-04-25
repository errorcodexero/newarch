#pragma once
#include <iostream>
#include <string>
#include <vector>


class sync {
public:
	virtual void getMessage(const std::vector<std::string>&)=0;
	virtual void sendMessage()=0;
};
