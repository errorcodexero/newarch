#include "posedge_trigger_debounce.h"

using namespace std;

Posedge_trigger_debounce::Posedge_trigger_debounce():debounce({false}),trigger({}){}

bool Posedge_trigger_debounce::operator()(Time now,bool in){
	debounce.update(now,in);
	return trigger(debounce.get());
}

bool operator<(Posedge_trigger_debounce const& a,Posedge_trigger_debounce const& b){
	#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	POSEDGE_TRIGGER_DEBOUNCE_ITEMS(X)
	#undef X
	return 0;
}

bool operator==(Posedge_trigger_debounce const& a,Posedge_trigger_debounce const& b){
	#define X(A,B) if(a.B!=b.B) return 0;
	POSEDGE_TRIGGER_DEBOUNCE_ITEMS(X)
	#undef X
	return 1;
}

bool operator!=(Posedge_trigger_debounce const& a,Posedge_trigger_debounce const& b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Posedge_trigger_debounce const& a){
	o<<"Posedge_trigger_debounce( ";
	#define X(A,B) o<<""#B<<":"<<a.B<<" ";
	POSEDGE_TRIGGER_DEBOUNCE_ITEMS(X)
	#undef X
	return o<<")";
}

#ifdef POSEDGE_TRIGGER_DEBOUNCE_TEST
int main(){
	Posedge_trigger_debounce d;
	assert(d(0,0)==0);
	assert(d(.01,1)==1);//transition here
	//cout<<d<<"\n";
	assert(d(.05,0)==0);
	//cout<<d<<"\n";
	assert(d(.2,1)==0);
	assert(d(.6,0)==0);//transition to zero here
	//cout<<d<<"\n";
	assert(d(1.2,0)==0);
	/*cout<<d<<"\n";
	cout.flush();*/
	assert(d(1.5,1)==1);
}
#endif
