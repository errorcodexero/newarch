#ifdef SIM_TEST
#include "toplevel.h"
#include "../util/type.h"
#include "nop.h"
#include "../executive/step.h"
#include "../executive/chain.h"
#include "../executive/teleop.h"
#include <math.h>
#include "main.h"
#include "../util/point.h"
#include "../util/util.h"

using namespace std;

template<typename T,size_t LEN>
array<pair<size_t,T>,LEN> enumerate(array<T,LEN> a){
	array<pair<size_t,T>,LEN> r;
	for(auto i:range(LEN)){
		r[i]=make_pair(i,a[i]);
	}
	return r;
}

template<typename Input>
struct Nop_sim{
	template<typename Output>
	void update(Time,bool,Output){}

	Input get()const{
		auto e=examples((Input*)0);
		assert(e.size());
		return *begin(e);
	}
};

template<typename T>
ostream& operator<<(ostream& o,Nop_sim<T> const& a){
	return o<<type(a);
}

using Pump_sim=Nop_sim<Pump::Input>;
using Lights_sim=Nop_sim<Lights::Input>;

struct Drivebase_sim{
	using Input=Drivebase::Input;
	using Output=Drivebase::Output;
	
	Point position; //x,y are in distance in feet, theta is in radians (positive is counterclockwise from straight forward)
	Time last_time;
	Drivebase::Distances distances;

	void update(Time t,bool enable,Output out){
		static const double POWER_TO_SPEED = 6.5 * 12;//speed is in/s assuming low gear
		Time dt=t-last_time;
		last_time=t;
		if(!enable) return;
		Drivebase::Speeds speeds = {out.l * POWER_TO_SPEED, out.r * POWER_TO_SPEED};
		Drivebase::Distances ddistances = {speeds.l * dt, speeds.r * dt};
		double avg_dist_traveled = mean(ddistances.l,ddistances.r);
		double dtheta = ((out.l-out.r)*POWER_TO_SPEED*dt)/Robot_constants::ROBOT_WIDTH;
		/*
			Angle is calculated as the difference between the two sides' powers divided by 2 --   (out.l - out.r) / 2
			That is then converted to a distance   --   ((out.l - out.r) / 2) * POWER_TO_SPEED * dt
			That distance is then converted to an angle -- ((((out.l - our.r) / 2) * POWER_TO_SPEED * dt) * 2) / ROBOT_WIDTH
		*/
		double dy = avg_dist_traveled * cosf(position.theta);
		double dx = avg_dist_traveled * sinf(position.theta);
		distances += ddistances;
		position += {dx,dy,dtheta};
	}
	Input get()const{
		auto d = Digital_in::_0;
		auto p = make_pair(d,d);
		Drivebase::Input in = {Drivebase::Input{
			{0,0,0,0},p,p,distances,0.0
		}};
		return in;
	}

	Drivebase_sim():position({}),last_time(0),distances({0,0}){}

};

ostream& operator<<(ostream& o,Drivebase_sim const& a){
	return o << "Drivebase_sim(" << a.position << ")";
}

struct Toplevel_sim{
	using Input=Toplevel::Input;
	using Output=Toplevel::Output;

	#define X(A,B,C) A##_sim B;
	TOPLEVEL_ITEMS
	#undef X

	void update(Time t,bool enable,Output out){
		#define X(A,B,C) B.update(t,enable,out.B);
		TOPLEVEL_ITEMS
		#undef X
	}

	Input get()const{
		return {
			#define X(A,B,C) B.get(),
			TOPLEVEL_ITEMS
			#undef X
		};
	}
};

template<typename SIMULATOR,typename DEVICE>
void simulate(SIMULATOR sim,DEVICE device){
	static const Time TIMESTEP=.01;//this will probably change to .01 or .02 later.
	auto in=sim.get();
	for(Time t=0;t<2;t+=TIMESTEP){
		//TODO: Make the simulator go 2014 style where you only get "significant" changes printed
		//device.estimator.update(
		
	}
}

template<typename F>
void visit(F f,Toplevel_sim a){
	#define X(A,B,C) f(""#B,a.B);
	TOPLEVEL_ITEMS
	#undef X
}

template<typename F,typename T>
void visit(F f,T t){
	f.terminal(t);
}

template<typename F,typename T>
void visit(F,Nop_sim<T>){}

/*void visit(auto f,Encoder_info const& a){

}*/

template<typename T,size_t LEN>
void visit(auto f,array<T,LEN> const& a){
	//mapf([&](auto p){ f(as_string(p.first),p.second); },enumerate(a));
	for(auto p:enumerate(a)){
		f(as_string(p.first),p.second);
	}
}

template<typename A,typename B>
void visit(auto f,pair<A,B> p){
	f("first",p.first);
	f("second",p.second);
}

#define VISIT(A,B) f(""#B,a.B);

void visit(auto f,Drivebase::Input const& a){
	DRIVEBASE_INPUT(VISIT)
}

template<typename F>
void visit(F f,Toplevel::Input const& a){
	#define X(A,B,C) f(""#B,a.B);
	TOPLEVEL_ITEMS
	#undef X
}

void visit(auto f,Nop::Input){}

void visit(auto f,Toplevel::Status_detail a){
	#define X(A,B,C) f(""#B,a.B);
	TOPLEVEL_ITEMS
	#undef X
}

void visit(auto f,Robot_outputs a){
	#define X(B) f(""#B,a.B);
	X(pwm) 
	X(solenoid) 
	X(relay) 
	X(digital_io) 
	X(talon_srx) 
	X(driver_station)
	X(pump)
	#undef X
}

struct Display{
	int indent_level=0;

	string get_label(auto a)const{
		return type(a);
	}

	/*auto get_label(int a)const{ return to_string(a); }
	auto get_label(bool a)const{ return to_string(a); }
	auto get_label(double a)const{ return to_string(a); }*/
	
	void indent()const{
		for(auto _:range(indent_level)){
			(void)_;
			cout<<"\t";
		}
	}

	void operator()(string label,auto a)const{
		indent();
		cout<<label<<"\n";
		visit(Display{indent_level+1},a);
	}

	void terminal(auto a)const{
		indent();
		cout<<a<<"\n";	
	}
};

template<typename T>
void sim_display(T t){
	/*auto label=get_top(t);
	cout<<label<<"\n";*/
	Display{}("robot",t);
}

struct Dedup_print{
	string last;

	void operator()(string prefix,string msg){
		if(msg!=last){
			cout<<prefix<<msg<<"\n";
			last=msg;
		}
	}

	void operator()(string prefix,auto a){
		(*this)(prefix,as_string(a));
	}
};

struct Dedup2{
	string intro;
	map<string,string> last;

	struct Inner{
		Dedup2 *parent;
		string prefix;

		void operator()(string s,auto a){
			visit(
				Inner{parent,prefix+"."+s},
				a
			);
		}

		void terminal(auto a){
			auto s=as_string(a);
			if(s!=parent->last[prefix]){
				cout<<parent->intro<<prefix<<":"<<s<<"\n";
				parent->last[prefix]=s;
			}
		}
	};

	void operator()(string intro,auto a){
		this->intro=intro;
		visit(
			//*this,prefix,
			/*[](auto prefix1,auto value){
				cout<<"p1:"<<prefix1<<":"<<value;
			}*/
			Inner{this,""},
			a
		);
	}
};

int main(){
	/*{
		Toplevel_sim sim;
		sim_display(sim);
		sim_display(sim.get());
		sim_display(example((Toplevel::Output*)0));
	}*/

	//Dedup_print mode,outp,inp,statusp;
	{//NOTE: All this is calibrated for use with side gear auto scoring (specifically the loading station side)
		Dedup2 mode,outp,inp,statusp,panel,drive_sim;
		Toplevel_sim sim;
		Main m;

		Robot_inputs robot_inputs;
		{
			Robot_inputs all;	
			all.robot_mode.autonomous = true;
			all.robot_mode.enabled = true;
			all.ds_info.alliance = Alliance::RED;
			all.joystick[Panel::PORT].axis[6] = -0.58;//should be loading station side (auto selector 5)
			robot_inputs = m.toplevel.input_reader(all,sim.get());
		}
		robot_inputs.robot_mode.autonomous = true;
		robot_inputs.robot_mode.enabled = true;
	
		static const Time TIMESTEP = .05;
		static const Time AUTO_LENGTH = 15;
		for(Time t = 0; t < AUTO_LENGTH; t += TIMESTEP){//set to 15 seconds for autonomous testing.
			robot_inputs.now = t;
			//cout << "Main " << m << "\n";
			inp(as_string(t)+"\tin",sim.get());
			Robot_outputs out = m(robot_inputs);
			//cout << "Mode: " <<m.mode << "\n";	
			drive_sim(as_string(t)+"\tdrive_sim",sim.drive);
			panel(as_string(t)+"\tpanel",interpret_oi(robot_inputs.joystick[Panel::PORT]));
			mode(as_string(t)+"\tmode",m.mode);
			//auto out=example((Toplevel::Output*)0);
			/*Toplevel::Goal goal;
			goal.drive.left=1;
			goal.drive.right=1;*/
			Toplevel::Status_detail status_detail = m.toplevel.estimator.get();
			statusp(as_string(t)+"\tstatus_detail",status_detail);
			//auto out=control(status_detail,goal);*/
			//cout <<"out "  << out << "\n";
			outp(as_string(t)+ "\tout",out);
			sim.update(t,robot_inputs.robot_mode.enabled,m.toplevel.output_applicator(out));
			m.toplevel.estimator.update(t,sim.get(),m.toplevel.output_applicator(out));
			robot_inputs = m.toplevel.input_reader(robot_inputs,sim.get());
			robot_inputs.robot_mode.autonomous = true;//override this for now
			robot_inputs.robot_mode.enabled = true;
#ifdef NEED_PIXY_CAM		       
			robot_inputs.camera.blocks = {{0,155,0,100,100}};//for align
#endif
		}
	}

	cout<<"\n\n\n=====================================================================================\n\n\n";

	{//NOTE: All this is calibrated for use for Align
		Dedup2 mode,outp,inp,statusp,panel,drive_sim;
		Toplevel_sim sim;
		Main m{Executive{Chain{
			Executive{Teleop()}
		}}};

		Robot_inputs robot_inputs;
		{
			Robot_inputs all;	
			all.robot_mode.autonomous = true;
			all.robot_mode.enabled = true;
			all.ds_info.alliance = Alliance::RED;
			robot_inputs = m.toplevel.input_reader(all,sim.get());
		}
		robot_inputs.robot_mode.autonomous = true;
		robot_inputs.robot_mode.enabled = true;

		static const Time TIMESTEP = .05;
		static const Time AUTO_LENGTH = 15;
		for(Time t = 0; t < AUTO_LENGTH; t += TIMESTEP){//set to 15 seconds for autonomous testing.
			robot_inputs.now = t;
			inp(as_string(t)+"\tin",sim.get());
			Robot_outputs out = m(robot_inputs);
			drive_sim(as_string(t)+"\tdrive_sim",sim.drive);
			panel(as_string(t)+"\tpanel",interpret_oi(robot_inputs.joystick[Panel::PORT]));
			mode(as_string(t)+"\tmode",m.mode);
			Toplevel::Status_detail status_detail = m.toplevel.estimator.get();
			statusp(as_string(t)+"\tstatus_detail",status_detail);
			outp(as_string(t)+ "\tout",out);
			sim.update(t,robot_inputs.robot_mode.enabled,m.toplevel.output_applicator(out));
			m.toplevel.estimator.update(t,sim.get(),m.toplevel.output_applicator(out));
			robot_inputs = m.toplevel.input_reader(robot_inputs,sim.get());
			robot_inputs.robot_mode.autonomous = true;//override this for now
			robot_inputs.robot_mode.enabled = true;

			{
				//set up the initial location of the blocks
				static const int MISALIGNED = 10;// px
#ifdef NEED_PIXY_CAM
				robot_inputs.camera.blocks = {
					{0,Block_pr::LEFT + MISALIGNED,0,40,40}, //left tape
					{0,Block_pr::RIGHT + MISALIGNED,0,30,30}, //right tape
					{0,0,30,20,20}, //random other
					{0,250,14,10,10} //random other
				};
			
				//shift the blocks according to the robot's angle
				static const double PIXELS_PER_DEGREE = (double)Pixy::Block::max_x / (double)Camera::FOV; // px/degree
				int rotate = sim.drive.position.theta * (180 /PI) * PIXELS_PER_DEGREE; //px how much to shift the x values of the blocks as the robot turns
				for(Pixy::Block& a: robot_inputs.camera.blocks){
					a.x = max((int)a.x - rotate, 0); //- becase image shifts left if robot turns clockwise
				}
#endif
				
			}
			
		}
	}
	return 0;	
}

#endif
