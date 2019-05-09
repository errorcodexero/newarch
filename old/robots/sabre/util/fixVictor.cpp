#include "fixVictor.h"

using namespace std;

double pwmToVel(double x){
	return (44.83294483*pow(x,3))-(97.61072261*pow(x,2))+(72.06332556*x)-6.106666667;
}

double velToPwm(double x){
	return (.001514415*pow(x,3))-(.0217689459*pow(x,2))+(.0964330594*x)+.0969951588;
}


typedef double (*FF)(double);

double error(FF fwd, FF rev){
	double s = 0;
	int slices = 100;
	for(int i = 0;i<slices;i++){
		double at = i/(slices-1)*2-1;
		s += fabs(rev(fwd(at))-at);
	}
	return s;
}

float adjust_for_victor(float f){
	if(fabs(f)<.05) return 0;
	double out=pwmToVel(fabs(f))/13.2;
	if(f<0) return -out;
	return out;
}

#ifdef FIXVICTOR_TEST
int main(){
	cout<<error(pwmToVel,velToPwm)<<endl;
	cout<<velToPwm(pwmToVel(.1))<<endl<<velToPwm(pwmToVel(.2))<<endl<<velToPwm(pwmToVel(.3))<<endl<<velToPwm(pwmToVel(.4))<<endl<<velToPwm(pwmToVel(.5))<<endl<<velToPwm(pwmToVel(.6))<<endl<<velToPwm(pwmToVel(.7))<<endl<<velToPwm(pwmToVel(.8))<<endl<<velToPwm(pwmToVel(.9))<<endl<<velToPwm(pwmToVel(1))<<endl;
	for(float x=-1;x<=1.05;x+=.1){
		cout<<x<<"\t"<<adjust_for_victor(x)<<"\n";
	}
}
#endif
