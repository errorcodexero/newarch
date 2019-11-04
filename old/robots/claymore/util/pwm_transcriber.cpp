#include "pwm_transcriber.h"

using namespace std;

#ifdef PWM_TRANSCRIBER_TEST

int main(){
	PWM_transcriber t;
	t.add("climbing",set<bool>{false,true});
	t.add("enabled",set<bool>{false,true});
	t.add("height",set<int>{0,1,2,3,4,5,6,7,8,9,10});
	
	cout<<t<<endl<<endl;

	cout<<t.map("climbing",false)<<endl;
	cout<<t.map("climbing",true)<<endl;
	cout<<t.map("height",5)<<endl;
	cout<<t.map("enabled",true)<<endl<<endl;
	
	cout<<t.transcribe("height",10)<<endl;	
}

#endif
