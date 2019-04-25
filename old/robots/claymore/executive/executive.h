#ifndef EXECUTIVE_H
#define EXECUTIVE_H

#include <memory>
#include <typeindex>
#include "../control/toplevel.h"
#include "../input/panel.h"
#include "../util/interface.h"
#include "../util/type.h"

#define NEXT_MODE_INFO_ITEMS(X)\
	X(bool,autonomous)\
	X(bool,autonomous_start)\
	X(Toplevel::Status_detail,status)\
	X(Time,since_switch)\
	X(Panel,panel)\
	X(Robot_inputs,in)
DECLARE_STRUCT(Next_mode_info,NEXT_MODE_INFO_ITEMS)
Next_mode_info rand(Next_mode_info*);
Next_mode_info example(Next_mode_info*);
std::vector<Next_mode_info> examples(Next_mode_info*);
std::ostream& operator<<(std::ostream&,Next_mode_info const&);

#define RUN_INFO_ITEMS(X)\
	X(Robot_inputs,in)\
	X(Joystick_data,driver_joystick)\
	X(Panel,panel)\
	X(Toplevel::Status_detail,status)
DECLARE_STRUCT(Run_info,RUN_INFO_ITEMS)
Run_info rand(Run_info*);
Run_info example(Run_info*);
std::vector<Run_info> examples(Run_info*);
std::ostream& operator<<(std::ostream&,Run_info const&);

class Executive_interface;

class Executive{
	std::unique_ptr<Executive_interface> impl;

	public:
	Executive(Executive const&);
	explicit Executive(Executive_interface const&);

	Executive next_mode(Next_mode_info);
	Toplevel::Goal run(Run_info);
	Executive_interface const& get()const;

	//bool operator<(Executive const&);
	Executive& operator=(Executive const&);
	friend bool operator==(Executive const&,Executive const&);
	friend std::ostream& operator<<(std::ostream&,Executive const&);
};
bool operator<(Executive const&,Executive const&);
bool operator==(Executive const&,Executive const&);

struct Executive_interface {
	virtual Executive next_mode(Next_mode_info)=0;
	virtual Toplevel::Goal run(Run_info)=0;

	virtual std::unique_ptr<Executive_interface> clone()const=0;
	virtual bool operator<(Executive_interface const&)const=0;
	virtual bool operator==(Executive const&)const=0;
	virtual void display(std::ostream&)const=0;
	virtual void diff(std::ostream&,Executive_interface const&)const=0;
};

bool operator!=(Executive const&,Executive const&);
std::ostream& operator<<(std::ostream&,Executive const&);

template<typename T>
struct Executive_impl:Executive_interface{
	T const& self()const{ return static_cast<T const&>(*this); }

	std::unique_ptr<Executive_interface> clone()const{
		return std::make_unique<T>(self());
	}

	bool operator<(Executive_interface const& a)const{
		auto t1=std::type_index(typeid(*this));
		auto t2=std::type_index(typeid(a));
		if(t1<t2){
			return 1;
		}
		if(t1>t2){
			return 0;
		}
		T const& b=dynamic_cast<T const&>(a);
		return this->operator<(b);
	}

	virtual bool operator<(T const& t)const{
		if(*this==t) return 0;
		assert(0);//FIXME
	}

	bool operator==(Executive const& a)const{
		try{
			T const& b=dynamic_cast<T const&>(a.get());
			return this->operator==(b);
		}catch(std::bad_cast const&){
			return 0;
		}
	}

	virtual bool operator==(T const&)const=0;

	void display(std::ostream& o)const{
		o<<type(self());
	}

	void diff(std::ostream& o,Executive_interface const& a)const{
		try{
			T const& b=dynamic_cast<T const&>(a);
			return diff(o,b);
		}catch(std::bad_cast const&){
			display(o);
			o<<" -> ";
			a.display(o);
		}
	}

	virtual void diff(std::ostream&,T const&)const{
		nyi
	}
};

#endif
