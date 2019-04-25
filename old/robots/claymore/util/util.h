#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include<string>
#include<map>
#include<iostream>
#include<vector>
#include<sstream>
#include<set>
#include<memory>
#include "interface.h"

#ifndef PI
#define PI 3.14159265
#endif

#define PRINT(x) std::cout<<""#x<<":"<<(x)<<"\n";
#define nyi { std::cout<<"nyi "<<__FILE__<<":"<<__LINE__<<"\n"; exit(44); }

bool approx_equal(double,double);

int write_file(std::string const& filename,std::string const& contents);
int read_file(std::string const& filename,std::string &out);//I don't like out parameters.

template<class T, class Compare>
const T& clamp(const T& v, const T& lo, const T& hi, Compare comp){ 
	return assert( !comp(hi, lo) ),
	comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template<class T>
const T& clamp(const T& v, const T& lo, const T& hi){
	return clamp( v, lo, hi, std::less<T>() );
}

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

double mean(double,double);
double sum(std::vector<double> const&);
double mean(std::vector<double> const&);
double stddev(std::vector<double> const&);

template<typename T>
std::vector<std::pair<size_t,T>> enumerate(std::vector<T>);

template<size_t LEN>
std::array<double,LEN> floats_to_doubles(std::array<float,LEN>);

template<size_t LEN>
double sum(std::array<double,LEN> const& a){
	double total=0;
	for(auto elem:a) total+=elem;
	return total;
}

template<size_t LEN>
double mean(std::array<double,LEN> const& a){
	return sum(a)/LEN;
}

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

template<typename T>
std::vector<T> to_vec(std::set<T> s){
	std::vector<T> r;
	for(auto a:s) r|=a;
	return r;
}

template<typename T>
T choose_random(std::vector<T> v){
	assert(v.size());
	return v[rand()%v.size()];
}

template<typename T>
T choose_random(std::set<T> a){
	return choose_random(to_vec(a));
}

template<typename T>
std::ostream& operator<<(std::ostream& o,std::unique_ptr<T> const& a){
	if(a.get()) return o<<*a.get();
	return o<<"NULL";
}

std::vector<size_t> range(size_t lim);

template<typename T>
std::set<T> to_set(std::vector<T> v){
	std::set<T> r;
	for(auto a:v) r|=a;
	return r;
}

template<typename T>
std::vector<T> take(size_t lim,std::vector<T> v){
	std::vector<T> r;
	for(size_t i=0;i<lim && i<v.size();i++){
		r|=v[i];
	}
	return r;
}

void print_lines(auto a){
	for(auto elem:a){
		std::cout<<elem<<"\n";
	}
}

auto cross(auto a,auto b){
	using A=decltype(*begin(a));
	using B=decltype(*begin(b));
	std::vector<std::pair<A,B>> r;
	for(auto a1:a) for(auto b1:b){
		r|=std::make_pair(a1,b1);
	}
	return r;
}

template<typename Func,typename Collection>
Collection filter(Func f,Collection c){
	Collection r;
	for(auto a:c){
		if(f(a)) r|=a;
	}
	return r;
}

template<typename T>
std::vector<T> operator-(std::vector<T> a,std::set<T> b){
	return filter([b](auto elem){ return b.count(elem)==0; },a);
}

template<typename Func,typename Collection>
auto mapf(Func f,Collection c){
	using T=typename std::remove_reference<decltype(f(*begin(c)))>::type;
	std::vector<T> out;
	for(auto a:c) out|=f(a);
	return out;
}

#define MAP(F,C) mapf([&](auto a){ return F(a); },C)

#endif
