#ifndef MAYBE_INLINE_H
#define MAYBE_INLINE_H

template<typename T>
class Maybe_inline{
	T t;
	bool valid;

	public:
	Maybe_inline():valid(0){}

	explicit Maybe_inline(T t1):t(t1),valid(1){}

	Maybe_inline& operator=(T t1){
		t=t1;
		valid=1;
		return *this;
	}

	operator bool()const{ return valid; }

	T& operator*(){
		assert(valid);
		return t;
	}

	T const& operator*()const{
		assert(valid);
		return t;
	}
};

template<typename T>
bool operator==(T a,Maybe_inline<T> b){
	if(b) return a==*b;
	return 0;
}

template<typename T>
bool operator==(Maybe_inline<T> a,T b){
	if(!a) return 0;
	return *a==b;
}

template<typename T>
bool operator!=(T a,Maybe_inline<T> b){
	return !(a==b);
}

template<typename T>
bool operator!=(Maybe_inline<T> a,T b){
	return !(a==b);
}

template<typename T>
std::ostream& operator<<(std::ostream& o,Maybe_inline<T> const& m){
	if(m) return o<<*m;
	return o<<"NULL";
}

#endif
