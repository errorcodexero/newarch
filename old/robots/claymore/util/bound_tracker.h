#ifndef BOUND_TRACKER_H
#define BOUND_TRACKER_H

#include <iosfwd>
#include <iostream>
#include "maybe.h"

template<typename T>
struct Bound_tracker{
	struct Bounds{
		T min, max;
		Bounds(T a, T b):min(a),max(b){}

		friend std::ostream& operator<<(std::ostream& o, Bounds a){
			o<<"min:"<<a.min;
			o<<" max:"<<a.max;
			return o;
		}		

	};
	
	Maybe<Bounds> bounds;
	
	Bound_tracker(){}
	
	void update(T const& v){
		if(bounds){
			//std::min and std::max don't work right on the cRIO. //TODO see if this works on the roboRIO
			//min=std::min(min,v);
			//max=std::max(max,v);
			if(v < (*bounds).min) (*bounds).min=v;
			if(v > (*bounds).max) (*bounds).max=v;
		}else{
			bounds = {v,v};
		}
	}

	bool has_data()const{
		return (bool)bounds;
	}
	
	Bounds get()const{
		return *bounds;
	}

	/*
	T min,max;
	bool data;
	
	Bound_tracker():data(false){
		//min = (strtod("NAN",NULL));//TODO
		//max = (strtod("NAN",NULL));//TODO
	}
	
	void update(T const& v){
		if(data){
			//std::min and std::max don't work right on the cRIO. //TODO see if this works on the roboRIO
			//min=std::min(min,v);
			//max=std::max(max,v);
			if(v<min) min=v;
			if(v>max) max=v;
		}else{
			min = v;
			max = v;
			data= true;
		}
	}
	*/
	//friend std::ostream& operator<<(std::ostream&,Bound_tracker<T>){
	
};

template<typename T>
std::ostream& operator<<(std::ostream& o,const Bound_tracker<T> a){
	o<<"Bound_tracker(";
	o<<a.bounds;
	return o<<")";
}

#endif
