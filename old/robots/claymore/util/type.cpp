#include "type.h"

#ifdef __GNUG__

#include<cstdlib>
#include<memory>
#include<cxxabi.h>

std::string demangle(const char *name){
	int status=0;//arbitrary value
	std::unique_ptr<char,void(*)(void*)> res{
		abi::__cxa_demangle(name,NULL,NULL,&status),
		std::free
	};
	return status?name:res.get();
}

#else

std::string demangle(const char *name){
	return name;
}

#endif

#ifdef TYPE_TEST
int main(){}
#endif
