#ifndef VISITABLE_H
#define VISITABLE_H

#include<vector>
#include "util.h"
#include "type.h"

template<typename T>
class Visitable{
};

struct Get_ptr{
	const void *x=0;

	template<typename T>
	void operator()(std::string const&,T const& a){
		x=&a;
	}

	void terminate(){
		x=0;
	}
};

struct Compare{
	const void *t;
	bool result;

	template<typename T>
	void operator()(std::string const&,T const& a){
		assert(t);
		T const& other=*static_cast<const T*>(t);
		result=(other==a);
	}

	void terminate(){
		assert(0);
	}
};

template<typename T>
bool operator==(Visitable<T> const& a,Visitable<T> const& b){
	auto a1=static_cast<T const&>(a);
	auto b1=static_cast<T const&>(b);

	size_t i=0;
	while(1){
		//store a const pointer to it
		//then in the next one, cast and compare it.
		Get_ptr get_ptr;
		visit(i,get_ptr,a1);
		if(!get_ptr.x) return 1;
		Compare cmp{get_ptr.x,0};
		visit(i,cmp,b1);
		if(cmp.result==0){
			return 0;
		}
		i++;
	}
}

struct Order{
	const void *t;
	int result;//-1=lt,0=eq,1=gt

	template<typename T>
	void operator()(std::string const&,T const& a){
		assert(t);
		T const& other=*static_cast<const T*>(t);
		if(other<a){
			result=-1;
			return;
		}
		if(a<other){
			result=1;
			return;
		}
		result=0;
	}

	void terminate(){
		assert(0);
	}
};

template<typename T>
bool operator<(Visitable<T> const& a,Visitable<T> const& b){
	auto a1=static_cast<T const&>(a);
	auto b1=static_cast<T const&>(b);

	size_t i=0;
	while(1){
		//store a const pointer to it
		//then in the next one, cast and compare it.
		Get_ptr get_ptr;
		visit(i,get_ptr,a1);
		if(!get_ptr.x) return 0;
		Order cmp{get_ptr.x,0};
		visit(i,cmp,b1);
		if(cmp.result==-1){
			return 1;
		}
		if(cmp.result==1){
			return 0;
		}
		i++;
	}
}

template<typename T>
bool operator>(Visitable<T> const& a,Visitable<T> const& b){
	return b<a;
}

template<typename T>
bool operator!=(Visitable<T> const& a,Visitable<T> const& b){
	return !(a==b);
}

template<typename T>
bool operator<=(Visitable<T> const& a,Visitable<T> const& b){
	return (a<b) || (a==b);
}

template<typename T>
bool operator>=(Visitable<T> const& a,Visitable<T> const& b){
	return (a>b) || (a==b);
}

struct Printer{
	std::ostream& o;
	bool done=0;

	Printer(std::ostream& o1):o(o1){}

	template<typename T>
	void operator()(std::string const& name,T const& t){
		o<<name<<":"<<t<<" ";
	}

	void terminate(){
		done=1;
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& o,Visitable<T> const& a_in){
	auto const& a=static_cast<T const&>(a_in);
	o<<type(a);
	o<<"( ";
	Printer printer{o};
	for(size_t i=0;!printer.done;i++){
		visit(i,printer,a);
	}
	return o<<")";
}

//The no-operation visitor.
struct Nop{
	bool done=0;

	template<typename T>
	void operator()(std::string const&,T const&){}

	void terminate(){
		done=1;
	}
};

template<typename T>
void visit1(size_t i,auto &f,Visitable<T> const& a){
	visit(i,f,static_cast<const T&>(a));
}

template<typename T>
size_t length(Visitable<T> const& a){
	size_t r=0;
	Nop nop;
	while(!nop.done){
		visit1(r,nop,a);
		r++;
	}
	return r;
}

#endif
