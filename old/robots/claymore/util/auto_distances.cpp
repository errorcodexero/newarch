#include "auto_distances.h"
#include <fstream>

using namespace std;

#define BASELINE   7 * 12 + 9.25
#define MIDDLE_PEG 114

Auto_distances::Auto_distances():
	baseline(BASELINE),
	middle_peg(MIDDLE_PEG)
{}

ostream& operator<<(ostream& o,Auto_distances const& a){
	o<<"Auto_distances( ";
	#define X(TYPE,NAME) o<<""#NAME<<":"<<a.NAME<<" ";
	AUTO_DISTANCES_ITEMS(X)
	#undef X
	return o<<")";
}

bool operator<(Auto_distances const& a,Auto_distances const& b){
	#define X(TYPE,NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	AUTO_DISTANCES_ITEMS(X)
	#undef X
	return 0;
}

bool operator==(Auto_distances const& a,Auto_distances const& b){
	#define X(TYPE,NAME) if(a.NAME!=b.NAME) return 0;
	AUTO_DISTANCES_ITEMS(X)
	#undef X
	return 1;
}

bool operator!=(Auto_distances const& a,Auto_distances const& b){
	return !(a==b);
}

#ifdef AUTO_DISTANCES_TEST
static const char *AUTO_DISTANCES_FILE="auto_distances.txt";
#else
static const char *AUTO_DISTANCES_FILE="/home/lvuser/auto_distances.txt";
#endif

Auto_distances read_auto_distances(){
	Auto_distances r;
	ifstream f(AUTO_DISTANCES_FILE);
	string s;
	//note: if file is incomplete/corrupted will just get as much data as it can.
	#define X(TYPE,NAME) if(f.good()){ getline(f,s); r.NAME=std::stod(s); }
	AUTO_DISTANCES_ITEMS(X)
	#undef X
	return r;
}

void write_auto_distances(Auto_distances c){
	ofstream f(AUTO_DISTANCES_FILE);
	#define X(TYPE,NAME) if(f.good()) f<<c.NAME<<"\n";
	AUTO_DISTANCES_ITEMS(X)
	#undef X
}

#ifdef AUTO_DISTANCES_TEST
Auto_distances rand(Auto_distances*){
	Auto_distances r;
	#define X(TYPE,NAME) r.NAME=rand()%10;
	AUTO_DISTANCES_ITEMS(X)
	#undef X
	return r;
}

void test_auto_distances_rw(){
	auto a=rand((Auto_distances*)nullptr);
	cout<<"a: "<<a<<endl;
	write_auto_distances(a);
	auto b=read_auto_distances();
	cout<<"b: "<<b<<endl;
	assert(a==b);
}

int main(){
	test_auto_distances_rw();
}
#endif
