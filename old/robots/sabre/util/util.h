#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include<string>
#include<map>
#include<iostream>
#include<vector>
#include<sstream>
#include<set>
#include "interface.h"

#ifndef PI
#define PI 3.14159265
#endif

Pwm_output pwm_convert(double);
double from_pwm(Pwm_output);

int write_file(std::string const& filename,std::string const& contents);
int read_file(std::string const& filename,std::string &out);//I don't like out parameters.

template<typename A,typename B>
std::ostream& operator<<(std::ostream& o,std::pair<A,B> const& p){
	return o<<"("<<p.first<<","<<p.second<<")";
}

template<typename K,typename V>
std::ostream& operator<<(std::ostream& o,std::map<K,V> const& m){
	o<<"{";
	for(typename std::map<K,V>::const_iterator at=m.begin();at!=m.end();++at){
		o<<*at;
	}
	return o<<"}";
}

template<typename T>
std::vector<T>& operator|=(std::vector<T>& v,T t){
	v.push_back(t);
	return v;
}

template<typename T,size_t LEN>
std::vector<T>& operator|=(std::vector<T>& v,std::array<T,LEN> const& a){
	for(auto elem:a) v|=elem;
	return v;
}

template<typename T>
std::ostream& operator<<(std::ostream& o,std::vector<T> const& v){
	o<<"[";
	for(typename std::vector<T>::const_iterator at=v.begin();at!=v.end();++at){
		o<<' '<<*at;
	}
	return o<<" ]";
}

template<typename T>
std::ostream& operator<<(std::ostream& o,std::set<T> const& v){
	o<<"{ ";
	for(auto const& a:v) o<<a<<" ";
	return o<<"}";
}

double sum(std::vector<double> const&);
double mean(std::vector<double> const&);
double stddev(std::vector<double> const&);

template<typename T>
std::string as_string(T const& t){
	std::stringstream ss;
	ss<<t;
	return ss.str();
}

double clip(double);

//this is not efficient.
template<typename T>
Maybe<T> parse_enum(std::vector<T> const& options,std::string const& value){
	for(unsigned i=0;i<options.size();i++){
		T opt=options[i];
		if(as_string(opt)==value) return Maybe<T>(opt);
	}
	return Maybe<T>();
}

std::vector<std::string> split(std::string const&,char);
std::vector<std::string> split(std::string const&);
double atof(std::string const&);
std::string inside_parens(std::string const&);

template<typename T,size_t LEN>
std::ostream& operator<<(std::ostream& o,std::array<T,LEN> const& a){
	o<<"[ ";
	for(auto const& elem:a) o<<elem<<" ";
	return o<<"]";
}

template<typename T>
std::set<T>& operator|=(std::set<T> &a,T t){
	a.insert(t);
	return a;
}

template<typename T>
std::set<T> operator|(std::set<T> a,std::set<T> const& b){
	for(auto elem:b) a|=elem;
	return a;
}

template<typename T>
T example(T* t){
	auto e=examples(t);
	assert(e.size());
	return *begin(e);
}

template<size_t LEN>
bool operator<(std::bitset<LEN> a,std::bitset<LEN> b){
	for(size_t i=0;i<LEN;i++){
		if(a[i]<b[i]) return 1;
		if(b[i]<a[i]) return 0;
	}
	return 0;
}

#endif
