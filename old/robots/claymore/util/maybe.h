#ifndef MAYBE_H
#define MAYBE_H

#include<cassert>
#include<iostream>

template<typename T>
class Maybe{
	T *t;

	public:
	Maybe():t(NULL){}

	explicit Maybe(T const& t1):t(new T(t1)){}

	Maybe(Maybe<T> const& m):t(NULL){
		if(m.t) t=new T(*m.t);
	}

	Maybe& operator=(Maybe const& m){
		if(t){
			if(m.t){
				*t=*m.t;
			}else{
				delete t;
				t=NULL;
			}
		}else{
			if(m.t){
				t=new T(*m.t);
			}
		}
		return *this;
	}

	Maybe& operator=(T const& a){
		if(t){
			*t=a;
		}else{
			t=new T(a);
		}
		return *this;
	}

	~Maybe(){
		delete t;
		t=NULL;
	}

	operator bool()const{ return !!t; }

	T& operator*(){
		assert(t);
		return *t;
	}

	T const& operator*()const{
		assert(t);
		return *t;
	}
};

template<typename T>
bool operator==(T a,Maybe<T> b){
	if(!b) return 0;
	return a==*b;
}

template<typename T>
bool operator==(Maybe<T> a,T b){
	return b==a;
}

template<typename T>
bool operator!=(T a,Maybe<T> b){
	return !(a==b);
}

template<typename T>
bool operator!=(Maybe<T> a,T b){
	return b!=a;
}

template<typename T>
std::ostream& operator<<(std::ostream& o,Maybe<T> const& m){
	if(m){
		return o<<*m;
	}else{
		return o<<"NULL";
	}
}

#endif
