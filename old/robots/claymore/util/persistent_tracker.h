#ifndef PERSISTENT_TRACKER_H
#define PERSISTENT_TRACKER_H

#include "maybe.h"
#include <vector>

template<typename T>
class Persistent_tracker{
	unsigned keep;
	std::vector<T> values;

	public:
	explicit Persistent_tracker(unsigned a):keep(a){}
	explicit Persistent_tracker():Persistent_tracker<T>::Persistent_tracker(5){}

	void update(T v){
		if(values.size() > keep){
			values.erase(values.begin());
		}
		values.push_back(v);
	}
	
	Maybe<T> average()const{
		if(values.empty()) return Maybe<T>();
		Maybe<T> average;
		T sum = {}; 
		for(T a: values){
			sum = sum + a;
		}
		average = sum / values.size();
		return average;
	}	

	Maybe<T> max()const{
		if(values.empty()) return Maybe<T>();
		Maybe<T> max;
		for(T a: values){
			if(!max || a > *max) max = a;
		}
		return max;
	}
	
	Maybe<T> min()const{
		if(values.empty()) return Maybe<T>();
		Maybe<T> min;
		for(T a: values){
			if(!min || a < *min) min = a;
		}
		return min;
	}

	std::vector<T> get()const{
		return values;
	}

	void discard(unsigned i){
		assert(i < values.size());
		values.erase(values.begin() + i);
	}
	
	template<typename U>
	friend std::ostream& operator<<(std::ostream&,Persistent_tracker<U> const&);
};

template<typename T>
std::ostream& operator<<(std::ostream& o,Persistent_tracker<T> const& a){
	o<<"Persistent_tracker(";
	o<<"keep:"<<a.keep;
	o<<" values:"<<a.values;
	o<<")";
	return o;//TODO
}

/*
template<typename T>
Maybe<T> Persistent_tracker<T>::max()const{
	if(values.empty()) return Maybe<T>();
	Maybe<T> max;
	for(T a: values){
		if(!max || a > *max) max = a;
	}
	return max;
}

template<typename T>
Maybe<T> Persistent_tracker<T>::min()const{
	if(values.empty()) return Maybe<T>();
	Maybe<T> min;
	for(T a: values){
		if(!min || a < *min) min = a;
	}
	return min;
}

*/

#endif
