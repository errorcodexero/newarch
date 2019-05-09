#include "util.h"
#include<fstream>
#include<cmath>
#include<stdlib.h>

using namespace std;

//static const unsigned char PWM_MAX=206,PWM_CENTER=128,PWM_MIN=50;//was 56 to make same as WPIlib
//probably want to decouple the deadzones from the rest of this.
Pwm_output pwm_convert(double p){
//going to emulate this so that we don't need to recalibrate stuff.
//	    void SetBounds(INT32 max, INT32 deadbandMax, INT32 center, INT32 deadbandMin, INT32 min);
//    SetBounds(206, 131, 128, 125, 56);
	/*double range=PWM_MAX-PWM_MIN;
	double range_multiplier=range/2;
	double out=range_multiplier*p+PWM_CENTER;
	if(out>125 && out<131) return PWM_CENTER;
	if(out>PWM_MAX) return PWM_MAX;
	if(out<PWM_MIN) return PWM_MIN;
	return (unsigned char)out;*/
	return p;
}

double from_pwm(Pwm_output a){
	/*double range=PWM_MAX-PWM_MIN;
	double range_multiplier=range/2;
	return (double)((int)a-PWM_CENTER)/range_multiplier;*/
	return a;
}

int write_file(string const& filename,string const& contents){
	ofstream file(filename.c_str());
	if(!file.is_open()) return 1;
	file<<contents;
	if(!file.good()) return 2;
	return 0;
}

int read_file(string const& filename,string &out){
	//slurp the whole file in
	ifstream file(filename.c_str());
	if(!file.is_open()) return 1;
	
	//the syntax of this next line is rather unfortunate.  If the extra parenthesis are removed it suddenly looks like a function declaration.
	string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	//could do some error checking here...
	out=s;
	return 0;
}

double sum(std::vector<double> const& v){
	double total=0;
	for(unsigned i=0;i<v.size();i++) total+=v[i];
	return total;
}

double mean(vector<double> const& v){
	if(v.size()==0) return strtod("NAN",NULL);
	return sum(v)/v.size();
}

vector<double> operator-(vector<double> v,double d){
	for(unsigned i=0;i<v.size();i++){
		v[i]-=d;
	}
	return v;
}

vector<double> square(vector<double> v){
	for(unsigned i=0;i<v.size();i++){
		v[i]*=v[i];
	}
	return v;
}

double stddev(vector<double> const& v){
	//could write a faster version of this.
	return sqrt(mean(square(
		v-mean(v)
	)));
}

double clip(double d){
	if(d > 1.0) return 1.0;
	if(d < -1.0) return -1.0;
	return d;
}

string combine_blanks(string s){
	//cout<<"comb"<<s<<"\n";
	stringstream ss;
	bool was_blank=1;
	for(unsigned i=0;i<s.size();i++){
		if(isblank(s[i])){
			if(!was_blank){
				ss<<s[i];
				was_blank=1;
			}
		}else{
			was_blank=0;
			ss<<s[i];
		}
	}
	return ss.str();
}

vector<string> split(string const& s,char c){
	vector<string> r;
	stringstream ss;
	for(unsigned i=0;i<s.size();i++){
		if(s[i]==c){
			r|=ss.str();
			ss.str("");
		}else{
			ss<<s[i];
		}
	}
	if(ss.str().size()) r|=ss.str();
	return r;
}

//this should probably go elsewhere, like util.cpp
vector<string> split(string const& s1){
	auto s=combine_blanks(s1);
	vector<string> r;
	stringstream ss;
	for(unsigned i=0;i<s.size();i++){
		if(isblank(s[i])){
			r|=ss.str();
			ss.str("");
		}else{
			ss<<s[i];
		}
	}
	if(ss.str().size()) r|=ss.str();
	return r;
}

double atof(string const& s){ return ::atof(s.c_str()); }

//could add some way to report error.
string inside_parens(string const& s){
	unsigned i;
	for(i=0;i<s.size() && s[i]!='(';i++) ;
	if(i==s.size()) return "";
	i++;//skip the '('
//	unsigned j;
//	for(j=0;j+i<s.size() && s[i+j]!=')';j++) ;
//	if(i+j<s.size()) return s.substr(i,j);
//	return "";

	int j=s.size();
	while(j>=0 && s[j]!=')') j--;
	if(j<0) return "";
	return 	s.substr(i,j-i);
}

#ifdef UTIL_TEST

#include<cassert>
#include<iostream>
#include "point.h"

void fileio_test(){
	string data="line1\nline2";
	string filename="f1.tmp";
	{
		int r=write_file(filename,data);
		assert(!r);
	}
	string out;
	{
		int r=read_file(filename,out);
		assert(!r);
	}
	assert(out==data);
}

int main(){
	fileio_test();

	vector<double> v{3,3,3};
	assert(approx_equal(mean(v),3));
	assert(approx_equal(stddev(v),0));
	v[1]=4;
	v.push_back(4);
	assert(approx_equal(mean(v),3.5));
	double s=stddev(v);
	assert(approx_equal(s,.5));
}
#endif
