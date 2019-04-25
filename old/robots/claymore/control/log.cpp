#include "log.h"
#include<typeinfo>

using namespace std;

bool file_exists(string const& name){
	ifstream f(name);
	return f.good();
}

string get_logfilename(){
	int i=0;
	auto name=[&](){
		stringstream ss;
		//ss<<"/media/sda1/log_"<<i<<".txt";
		return ss.str();
	};
	while(file_exists(name())){
		i++;
	}
	return name();
}

#define ROBOT_OUTPUTS(X) \
	X(pwm) \
	X(solenoid) \
	X(relay) \
	X(digital_io) \
	X(talon_srx) \
	X(navx) \
	X(driver_station) \
	X(pump)

#define LOG_INNER(NAME) log(o,header,prefix+"_"#NAME,a.NAME);

#define DIGITAL_INPUTS(X) X(in) X(encoder)

#define JOYSTICK_DATA(X) X(axis) X(button) X(pov)

#define ROBOT_MODE(X) X(autonomous) X(enabled)

#define ROBOT_INPUTS(X) \
	X(robot_mode) \
	X(now) \
	X(ds_info) \
	X(joystick) \
	X(digital_io) \
	X(analog) \
	X(talon_srx) \
	X(navx) \
	X(driver_station) \
	X(orientation) \
	X(current) \
	X(pump) \

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
void visit(Func f,Alliance const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,DS_info const& a){
	#define X(TYPE,NAME) f(""#NAME,a.NAME);
	DS_INFO_ITEMS(X)
	#undef X
}

template<typename Func>
void visit(Func f,Navx_input const& a){
	#define X(TYPE,NAME,...) f(""#NAME,a.NAME);
	NAVX_INPUT_ITEMS(X)
	#undef X
}

template<typename Func>
void visit(Func f,Navx_output const& a){
	#define X(TYPE,NAME,...) f(""#NAME,a.NAME);
	NAVX_OUTPUT_ITEMS(X)
	#undef X
}

template<typename Func>
void visit(Func f,Talon_srx_input const& a){
	#define X(TYPE,NAME) f(""#NAME,a.NAME);
	TALON_SRX_INPUT_ITEMS(X)
	#undef X
}

template<typename Func>
void visit(Func f,Talon_srx_output::Mode const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Talon_srx_output const& a){
	#define X(TYPE,NAME) f(""#NAME,a.NAME);
	TALON_SRX_OUTPUT_ITEMS(X)
	#undef X
}

template<typename Func>
void visit(Func f,Pump_input const& a){
	#define X(TYPE,NAME) f(""#NAME,a.NAME);
	PUMP_INPUT_ITEMS(X)
	#undef X
}

template<typename Func>
void visit(Func f,Pump_output const& a){
	#define X(TYPE,NAME) f(""#NAME,a.NAME);
	PUMP_OUTPUT_ITEMS(X)
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

#ifdef NEED_PIXY_CAM
template<typename Func>
void visit(Func f,Camera const& r){
	#define CAMERA_ITEMS(X) X(FOV) X(enabled) X(blocks)
	CAMERA_ITEMS(VISIT)
}
#endif

template<typename Func>
void visit(Func f,PID_values const& r){
	#define PID_VALUE_ITEMS(X) X(p) X(i) X(d) X(f)
	PID_VALUE_ITEMS(VISIT)
}

template<typename Func>
void visit(Func f,Driver_station_output const& r){
	#define DRIVER_STATION_OUTPUT(X) X(digital)
	DRIVER_STATION_OUTPUT(VISIT)
}

template<typename Func>
void visit(Func f,Driver_station_input const& r){
	#define DRIVER_STATION_INPUT(X) X(analog) X(digital)
	DRIVER_STATION_OUTPUT(VISIT)
}

template<typename Func>
void visit(Func f,Robot_outputs const& r){
	ROBOT_OUTPUTS(VISIT)
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

template<typename Func, typename T>
void visit(Func f,vector<T> v){
	f.terminal(v);
}

template<typename Func>
void visit(Func f,string s){
	f.terminal(s);
}

template<typename Func>
void visit(Func f,long int i){
	f.terminal(i);
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
void visit(Func f,Nop::Status const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Drivebase::Distances const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Drivebase::Encoder_ticks const& a){
	f.terminal(a);
}

template <typename Func>
void visit(Func f,Drivebase::Speeds const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Drivebase::Output const& a){
	f.terminal(a);
}


template<typename Func>
void visit(Func f,Drivebase::Status const& r){
	DRIVEBASE_STATUS(VISIT_2ND)
}

template<typename Func>
void visit(Func f,Lights::Status_detail const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Grabber::Status_detail const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Lifter::Status_detail const& a){
	f.terminal(a);
}

template<typename Func>
void visit(Func f,Pump::Status const& a){
	f.terminal(a);
}

template<typename Func,typename T1,typename T2>
void visit(Func f,std::pair<T1,T2> p){
	visit(f,p.first);
	visit(f,p.second);
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
