#include "debounce.h"

using namespace std;

#ifdef DEBOUNCE_TEST
int main(){
	Debounce<bool> d = {false};
	assert(d.get()==0);
	d.update(0,1);
	assert(d.get()==1);
	d.update(.2,0);
	assert(d.get()==1);
	d.update(.8,0);
	cout<<d<<"\n";
	assert(d.get()==0);
}
#endif
