#include "../input/joystick_linux.h"

#include<fcntl.h>
#include<string>
#include<iostream>
#include<errno.h>
#include<sstream>
#include<sys/types.h>
#include<linux/joystick.h>
#include<dirent.h>
#include "../util/util.h"
#include <unistd.h>
#include<cassert>

using namespace std;

//This ifdef is here because I haven't found a good way to tell Wind River Workbench not to compile a file.
#ifdef JOYSTICK_LINUX

//list of the names in a directory.
vector<string> dir(string name){
	DIR *d=opendir(name.c_str());
	if(!d) throw "could not open dir";
	vector<string> r;
	while(1){
		struct dirent *ent=readdir(d);
		if(!ent){
			int cr=closedir(d);
			assert(!cr);
			return r;
		}
		r|=string(ent->d_name);
	}
}

string js_event_str(unsigned char c){
	stringstream ss;
	if(c&JS_EVENT_INIT){
		ss<<"init ";
	}
	c&=~JS_EVENT_INIT;
	switch(c){
		case JS_EVENT_BUTTON:
			ss<<"button";
			break;
		case JS_EVENT_AXIS:
			ss<<"axis";
			break;
		default:
			ss<<"unknown";
	}
	return ss.str();
}

ostream& operator<<(ostream& o,struct js_event a){
	o<<"js_event(";
	o<<"type="<<js_event_str(a.type);//(int)a.type;
	o<<" time="<<a.time;
	o<<" number="<<(int)a.number;
	o<<" value="<<a.value;
	return o<<")";
}

/*
The actual outputs that show up on linux for the Logitech gamepad:
name appears as "Generic X-Box pad"
with the "mode" button off:
axes:
0 left l/r
1 left y
2 left trigger
3 right x
4 right y
5 right trigger
6 dpad x
7 dpad y
8 
9 

mode buton on: swaps dpad & left stick & makes left stick not do graduations (full on or off)

buttons:
a0
b1
x2
y3
left upper 4
right upper 5
back 6
start 7
left joy click 9
right joy click 10
*/

ostream& operator<<(ostream& o,Joystick_data_linux const& a){
	o<<"Joystick_data_linux(";
	o<<a.name;
	o<<a.axis;
	o<<a.button;
	return o<<")";
}

vector<string> joysticks(){
	vector<string> v=dir("/dev/input"),r;
	for(vector<string>::iterator at=v.begin();at!=v.end();++at){
		if((*at)[0]=='j') r|=*at;
	}
	//if we wanted to be fancy, we could attempt to sort this by the same algorithm that the driver station uses.
	return r;
}

template<typename K,typename V>
V get(map<K,V> const& m,K key){
	typename map<K,V>::const_iterator f=m.find(key);
	if(f==m.end()) return 0;
	return f->second;
}

//translates into what the driver station would tell you.
Joystick_data as_joystick_data(Joystick_data_linux const& j){
	if(j.name!="Generic X-Box pad"){
		throw "Don't know about how this joystick type maps when used with the drivers' station software.";
	}
	Joystick_data r;
	for(unsigned i=0;i<8;i++){
		r.button[i]=get(j.button,i);
	}
	r.button[8]=get(j.button,9u);
	r.button[9]=get(j.button,10u);

	#define G(index) ((double)get(j.axis,(unsigned)index)/32767)
	for(unsigned i=0;i<2;i++){
		r.axis[i]=G(i);
		r.axis[3+i]=G(3+i);
	}
	r.axis[5]=G(6);
	r.axis[2]=(G(5)-G(2))/2;
	#undef G
	return r;
}

Joystick_linux::Joystick_linux(const char *path):fd(open(path,O_RDONLY)){
	if(fd<0) throw string("Error: Could not open joystick at ")+path;
	unsigned char axes=2,buttons=2;
	static const unsigned NAME_LENGTH=128;
	char name[NAME_LENGTH]="Unknown";
	ioctl(fd,JSIOCGAXES,&axes);
	ioctl(fd,JSIOCGBUTTONS,&buttons);
	ioctl(fd,JSIOCGNAME(NAME_LENGTH),name);
	//cout<<"Got name:"<<name<<"\n";
	fcntl(fd,F_SETFL,O_NONBLOCK);
	data.name=name;
}

Joystick_linux::~Joystick_linux(){
	close(fd);
}

Joystick_data_linux const& Joystick_linux::read(){
	if(fd<0) throw "Error: Joystick is not open.\n";
	bool done=0;
	while(!done){
		struct js_event js;
		errno=0;
		int r=::read(fd,&js,sizeof(struct js_event));
		if(r!=sizeof(struct js_event)){
			done=1;
		}else{
			unsigned char t=js.type&~JS_EVENT_INIT;
			if(t==JS_EVENT_BUTTON){
				data.button[js.number]=js.value;
			}else if(t==JS_EVENT_AXIS){
				data.axis[js.number]=js.value;
			}else{
				throw "Unknown event type!";
			}
		}
	}
	return data;
}
#endif

ostream& operator<<(ostream& o,Joystick_linux const& /*a*/){
	//todo:FIXME
	return o<<"Joystick_linux";
}

#ifdef JOYSTICK_LINUX_TEST
int main(){
	cout<<joysticks()<<"\n";
	try{
		Joystick_linux a("/dev/input/js0");
		for(unsigned i=0;1 || i<10;i++){
			cout<<a.read()<<"\n";
			usleep(1000*100);
		}
	}catch(string& s){
		cerr<<s<<"\n";
		return 1;
	}
}
#endif
