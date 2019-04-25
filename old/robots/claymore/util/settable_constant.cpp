#include "settable_constant.h"

using namespace std;	

#ifdef SETTABLE_CONSTANT_TEST

int main(){
	Settable_constant<double> a;
	cout<<a<<endl;
	a = 10;
	cout<<a<<endl;
	a = 5;
	cout<<a<<endl;
}

#endif
