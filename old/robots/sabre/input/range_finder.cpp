#include<iostream>

//This stuff is left over from experiments fall 2014 that used some IR rangefinders

using namespace std;

float newdistance(float x1, float y1, float x2, float y2, float x3);

float converttodistance(float value){
	if(value>.25&&value<=.35)return newdistance(.25,3,.35,2,value);
	if(value>.35&&value<=.66)return newdistance(.35,2,.66,1,value);
	if(value>.66&&value<=.79)return newdistance(.66,1,.79,.80,value);
	if(value>.79&&value<=1.01)return newdistance(.79,.80,1.01,.60,value);
	if(value>1.01&&value<=1.51)return newdistance(1.01,.60,1.51,.40,value);
	if(value>1.51)return newdistance(1.51,.40,2.51,.20,value);
	
	if(value<.26){
		return 3;
	}
	else if(value>=.26&&value<.51){
		return 2;
	}
	else if(value>=.51&&value<.73){
		return 1;
	}
	else if(value>=.73&&value<.81){
		return .8;
	}
	else if(value>=.81&&value<1.21){
		return .6;
	}
	else if(value>=1.21&&value<2.01){
		return .4;
	}
	else if(value>=2.01&&value<2.26){
		return 0;
	}
	else if(value>=2.26&&value<=2.6){
		return .2;
	}
	else
		return 0;	
}	
	
float newdistance(float x1, float y1, float x2, float y2, float x3){
	float m;
	m=(y2-y1)/(x2-x1);
	float b;
	b=y1-m*x1;
	return m*x3+b;
}
	
float timetowall(float value,float speed){	
	return converttodistance(value)/speed;
}
	
void getDistance(float value){
	cout<<converttodistance(value)<<"m\n";
}

//int main(){}
