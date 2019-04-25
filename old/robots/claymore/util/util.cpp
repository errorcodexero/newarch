#include "util.h"
#include<fstream>
#include<cmath>
#include<stdlib.h>

using namespace std;

bool approx_equal(double a,double b){
	return fabs(a-b)<.001;
}

vector<size_t> range(size_t lim){
	vector<size_t> r;
	for(size_t i=0;i<lim;i++) r|=i;
	return r;
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

template<typename T>
vector<pair<size_t,T>> enumerate(vector<T> a){
	vector<pair<size_t,T>> r;
	for(size_t i=0;i<a.size();i++){
		r|=make_pair(i,a[i]);
	}
	return r;
}

template<size_t LEN>
array<double,LEN> floats_to_doubles(array<float,LEN> a){
	array<double,LEN> r;
	for(size_t i=0;i<LEN;i++) r[i]=a[i];
	return r;
}

double mean(double a,double b) { return (a+b)/2; }

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
