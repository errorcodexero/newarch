#include "persistent_tracker.h"
#include "util.h"

using namespace std;	

#ifdef PERSISTENT_TRACKER_TEST

int main(){
	Persistent_tracker<int> a;
	for(int i: range(100)){
		cout<<a<<" avg:"<<a.average()<<" max:"<<a.max()<<" min:"<<a.min()<<" get:"<<a.get()<<"\n";
		a.update(i);
		for(unsigned i = 0; i < a.get().size(); i++){
			if(a.get()[i] % 2 == 0) a.discard(i);
		}
	}
}

#endif
