#ifndef MONITOR_H
#define MONITOR_H

#include<sstream>
#include<iostream>
#include "main.h"

template<typename T>
void print_diff(std::ostream& o,std::string s,T &a,T b){
	if(a!=b){
		o<<s<<"From "<<a<<" to "<<b<<"\n";
		a=b;
	}
}

template<typename T>
void print_diff(std::ostream& o,T &a,T b){
	print_diff(o,"",a,b);
}

template<typename T>
void print_diff_approx(std::ostream& o,T &a,T b){
	if(!approx_equal(a,b)){
		o<<"From "<<a<<" to "<<b<<"\n";
		a=b;
	}
}

void print_diff(std::ostream&,unsigned char&,unsigned char);
void print_diff(std::ostream&,Toplevel::Status&,Toplevel::Status const&);
void print_diff(std::ostream&,Main&,Main const&);
void print_diff(std::ostream&,Driver_station_output&,Driver_station_output const&);
void print_diff(std::ostream&,Robot_outputs&,Robot_outputs const&);
void print_diff(std::ostream&,Robot_inputs&,Robot_inputs const&);

template<typename T>
struct Monitor{
	Maybe<T> data;

	std::string update(T t){
		std::stringstream ss;
		if(data){
			print_diff(ss,*data,t);
		}else{
			ss<<t<<"\n";
		}
		data=t;
		return ss.str();
	}
};

template<>
struct Monitor<Toplevel::Estimator>{
	Toplevel::Status status;
	std::string update(Toplevel::Estimator);
};

template<>
struct Monitor<Main>{
	Main data;
	Monitor<Toplevel::Estimator> est;
	std::string update(Main);
};

#endif
