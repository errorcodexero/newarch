#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "countdown_timer.h"

template <typename T>
class Debounce{
	#define DEBOUNCE_ITEMS(X) \
		X(Countdown_timer,timer)\
		X(T,value) \
		X(Time,interval) 

	#define X(A,B) A B;
	DEBOUNCE_ITEMS(X)
	#undef X

	public:
	Debounce(T a):Debounce<T>::Debounce(a,.5){}
	Debounce(T a,Time b):value(a),interval(b){}


	T get()const{ return value; }
	void update(Time now,T v){
		timer.update(now,1);
		if(!timer.done()) return;
		if(v==value) return;
		value=v;
		timer.set(interval);
		timer.update(now,1);
	}

	template <typename S>
	friend bool operator<(Debounce<S> const&,Debounce<S> const&);
	template <typename S>
	friend bool operator==(Debounce<S> const&,Debounce<S> const&);
	template <typename S>
	friend std::ostream& operator<<(std::ostream&,Debounce<S> const&);
};
template<typename T>
bool operator<(Debounce<T> const& a,Debounce<T> const& b){
	#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	DEBOUNCE_ITEMS(X)
	#undef X
	return 0;
}

template<typename T>
bool operator==(Debounce<T> const& a,Debounce<T> const& b){
	return 1
	#define X(A,B) && a.B==b.B
	DEBOUNCE_ITEMS(X)
	#undef X
	;
}

template<typename T>
bool operator!=(Debounce<T> const& a,Debounce<T> const& b){
	return !(a==b);
}

template<typename T>
std::ostream& operator<<(std::ostream& o,Debounce<T> const& a){
	o<<"Debounce( ";
	#define X(A,B) o<<""#B<<":"<<a.B<<" ";
	DEBOUNCE_ITEMS(X)
	#undef X
	return o<<")";
}
#endif
