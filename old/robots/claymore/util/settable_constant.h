#ifndef SETTABLEP_CONSTANT_H
#define SETTABLE_CONSTANT_H

#include <cassert>
#include <iostream>

template<typename T>
class Settable_constant{
	T value;
	bool set;

	public:
	Settable_constant():set(false){}

	explicit Settable_constant(T v):value(v),set(true){}

	Settable_constant& operator=(T v){
		if(!set){
			value = v;
			set = true;
		}
		return *this;
	}

	operator bool()const{ return set; }

	T& operator*(){
		assert(set);
		return value;
	}

	T const& operator*()const{
		assert(set);
		return value;
	}
};

template<typename T>
bool operator==(T a,Settable_constant<T> b){
	if(b) return a==*b;
	return false;
}

template<typename T>
bool operator==(Settable_constant<T> a,T b){
	if(!a) return false;
	return *a==b;
}

template<typename T>
bool operator!=(T a,Settable_constant<T> b){
	return !(a==b);
}

template<typename T>
bool operator!=(Settable_constant<T> a,T b){
	return !(a==b);
}

template<typename T>
std::ostream& operator<<(std::ostream& o,Settable_constant<T> const& a){
	if(a) return o<<*a;
	return o<<"NULL";
}

#endif
