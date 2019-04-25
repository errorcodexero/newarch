#pragma once

#include "message_dest_file.h"
#include <ctime>

    
class messageDestDatedFile : public messageDestFile
{
public:
    messageDestDatedFile(const std::string &basedir)
    {
	std::array<char, 64> buffer ;
	time_t now ;
	tm *ltm ;
	
	time(&now) ;
	ltm = localtime(&now) ;

	strftime(&buffer[0], buffer.size(), "logfile-%m-%d-%Y-%H-%M-%S", ltm) ;
	
	std::string file = basedir ;
	file += "/" ;
	file += &buffer[0] ;

	setFile(file) ;
    }
} ;

