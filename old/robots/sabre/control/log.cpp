#include "log.h"
#include<typeinfo>

#define nyi { cout<<"nyi "<<__LINE__<<"\n"; exit(44); }

bool file_exists(string const& name){
	ifstream f(name);
	return f.good();
}

string get_logfilename(){
	int i=0;
	auto name=[&](){
		stringstream ss;
		ss<<"/media/sda1/log_"<<i<<".txt";
		return ss.str();
	};
	while(file_exists(name())){
		i++;
	}
	return name();
}

#define ROBOT_OUTPUTS(X) X(pwm) X(solenoid) X(relay) X(digital_io) X(talon_srx) X(panel_output) X(driver_station)

#define LOG_INNER(NAME) log(o,header,prefix+"_"#NAME,a.NAME);

#define DIGITAL_INPUTS(X) X(in) X(encoder)

#define JOYSTICK_DATA(X) X(axis) X(button) X(pov)

#define TALON_SRX_INPUT(X) \
	X(encoder_position) \
	X(fwd_limit_switch)\
	X(rev_limit_switch)\
	X(a)\
	X(b)\
	X(velocity)\
	X(current)

#define ROBOT_MODE(X) X(autonomous) X(enabled)

#define ROBOT_INPUTS(X) \
	X(robot_mode) \
	X(now) \
	X(joystick) \
	X(digital_io) \
	X(analog) \
	X(talon_srx) \
	X(current) \
	X(pump)

template<typename Func>
void visit(Func f,double d){
	f.terminal(d);
}

template<typename Func,typename T,size_t LEN>
void visit(Func f,array<T,LEN> const& a){
	for(size_t i=0;i<LEN;i++){
		f(i,a[i]);
	}
}

template<typename Func,typename T,size_t LEN>
void visit(Func f,Checked_array<T,LEN> const& a){
	for(size_t i=0;i<LEN;i++){
		f(i,a[i]);
	}
}

template<typename Func,size_t LEN>
void visit(Func f,bitset<LEN> const& a){
	for(size_t i=0;i<LEN;i++) f(i,a[i]);
}

template<typename Func>
void visit(Func f,Joystick_data const& a){
	#define X(NAME) f(""#NAME,a.NAME);
	JOYSTICK_DATA(X)
	#undef X
}

template<typename Func>
void visit(Func f,Talon_srx_input const& a){
	#define X(NAME) f(""#NAME,a.NAME);
	TALON_SRX_INPUT(X)
	#undef X
}

template<typename Func>
void visit(Func f,Digital_in a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Digital_inputs const& a){
	#define X(NAME) f(""#NAME,a.NAME);
	DIGITAL_INPUTS(X)
	#undef X
}

template<typename Func>
void visit(Func f,Robot_mode const& a){
	#define X(NAME) f(""#NAME,a.NAME);
	ROBOT_MODE(X)
	#undef X
}

#define VISIT(NAME) f(""#NAME,r.NAME);
#define VISIT_2ND(_,NAME) f(""#NAME,r.NAME);

template<typename Func>
void visit(Func f,Robot_inputs const& r){
	ROBOT_INPUTS(VISIT)
}

template<typename Func>
void visit(Func f,Relay_output const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Digital_out const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Talon_srx_output const& r){
	#define TALON_SRX_OUTPUTS(X) X(power_level)
	TALON_SRX_OUTPUTS(VISIT)
}

template<typename Func>
void visit(Func f,Panel_output const& r){
	#define PANEL_OUTPUTS(X) X(port) X(value)
	PANEL_OUTPUTS(VISIT)
}

template<typename Func>
void visit(Func f,Driver_station_output const& r){
	#define DRIVER_STATION_OUTPUT(X) X(digital)
	DRIVER_STATION_OUTPUT(VISIT)
}

template<typename Func>
void visit(Func f,Robot_outputs const& r){
	ROBOT_OUTPUTS(VISIT)
}

template<typename Func>
void visit(Func f,Collector::Status_detail const& r){
	COLLECTOR_ITEMS(VISIT_2ND)
}

template<typename Func>
void visit(Func f,Toplevel::Status_detail const& a){
	#define X(A,B,C) f(""#B,a.B);
	TOPLEVEL_ITEMS
	#undef X
}

/*template<typename Func,typename T>
void visit(Func f,T t){
	cout<<"t:"<<t<<"\n";
	cout<<"type:"<<typeid(t).name()<<" "<<sizeof(t)<<"\n";
	nyi
}*/

template<typename Func>
void visit(Func f,Motor_check::Status a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,float a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,bool b){
	f.terminal(b);
}

template<typename Func,typename T>
void visit(Func f,Maybe<T> const& m){
	//otherwise would have variable width
	f.terminal(m);
}

template<typename Func,typename T>
void visit(Func f,Maybe_inline<T> const& m){
	//otherwise would have variable width
	f.terminal(m);
}

template<typename Func>
void visit(Func f,int i){
	f.terminal(i);
}

template<typename Func>
void visit(Func f,Drivebase::Status const& r){
	DRIVEBASE_STATUS(VISIT_2ND)
}

template<typename Func>
void visit(Func f,Pump::Status const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Tilt::Status_detail const& r){
	#define TILT_STATUS_DETAIL(X) X(top) X(angle)
	TILT_STATUS_DETAIL(VISIT)
}

template<typename Func>
void visit(Func f,Front::Status_detail const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Climb_release::Status_detail const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Winch::Status_detail const& r){
	#define WINCH_STATUS_DETAIL(X) X(deployed)
	WINCH_STATUS_DETAIL(VISIT)
}

template<typename Func>
void visit(Func f,Sides::Input const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Shooter::Status_detail const& a){
	f.terminal(a);
}

struct Header{
	ostream& o;
	string name;

	template<typename T1,typename T2>
	void operator()(T1 const& label,T2 const& item)const{
		visit(Header{o,name+"_"+as_string(label)},item);
	}

	template<typename T>
	void terminal(T const&)const{
		o<<name<<",";
	}
};

struct Data_csv{
	ostream& o;

	template<typename T1,typename T2>
	void operator()(T1 const&/* label*/,T2 const& item)const{
		visit(*this,item);
	}

	template<typename T>
	void terminal(T const& t)const{
		o<<t<<",";
	}
};

/*struct Data_bin{
	ostream& o;
	template<typename T1,typename T2>
	void operator()(T1 const&,T2 const& item)const{
		visit(*this,item);
	}

	template<typename T>
	void terminal(T const& t)const{
		o.write((char*)&t,sizeof(t));
	}
};*/

Log::Log():Log(get_logfilename()){}

Log::Log(string const& filename):f(filename){
	if(f.good()){
		visit(Header{f,"in"},Robot_inputs{});
		visit(Header{f,"status"},example((Toplevel::Status_detail*)nullptr));
		visit(Header{f,"out"},Robot_outputs{});
		f<<"\n";
	}
}

void Log::operator()(Robot_inputs const& in,Toplevel::Status_detail const& status,Robot_outputs const& out){
	if(f.good()){
		stringstream ss;
		Data_csv d{ss};
		visit(d,in);
		visit(d,status);
		visit(d,out);
		ss<<"\n";
		f.write(ss.str().c_str(),ss.str().size());
		f.flush();
	}
}

#ifdef LOG_TEST
int main(){
	Log log("x1.txt");
	auto a=example((Toplevel::Status_detail*)nullptr);
	for(int i=0;i<100;i++){
		log(Robot_inputs{},a,Robot_outputs{});
	}
}
#endif
