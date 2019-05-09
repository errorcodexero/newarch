#ifndef CHECKED_ARRAY_H
#define CHECKED_ARRAY_H

#include<array>

//This is meant to be a drop-in replacement for std::array that does bounds checking

template<typename T,size_t LEN>
struct Checked_array{
	using Inner=std::array<T,LEN>;
	using iterator=typename Inner::iterator;
	using const_iterator=typename Inner::const_iterator;

	Inner inner;

	Checked_array& operator=(std::array<T,LEN> const& a){
		inner=a;
		return *this;
	}

	T& operator[](size_t i){
		assert(i<LEN);
		return inner[i];
	}

	T const& operator[](size_t i)const{
		assert(i<LEN);
		return inner[i];
	}

	iterator begin(){ return inner.begin(); }
	iterator end(){ return inner.end(); }
};

template<typename T,size_t LEN>
bool operator<(Checked_array<T,LEN> const& a,Checked_array<T,LEN> const& b){
	return a.inner<b.inner;
}

template<typename T,size_t LEN>
bool operator==(Checked_array<T,LEN> const& a,Checked_array<T,LEN> const& b){
	return a.inner==b.inner;
}

template<typename T,size_t LEN>
bool operator!=(Checked_array<T,LEN> const& a,Checked_array<T,LEN> const& b){
	return !(a==b);
}

template<typename T,size_t LEN>
std::ostream& operator<<(std::ostream& o,Checked_array<T,LEN> const& a){
	return o<<a.inner;
}

#endif
