#ifndef TYPE_H
#define TYPE_H

#include<string>
#include<typeinfo>

std::string demangle(const char*);

template<typename T>
std::string type(T const& t){
	return demangle(typeid(t).name());
}

#endif
