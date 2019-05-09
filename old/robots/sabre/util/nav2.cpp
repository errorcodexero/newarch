#include "nav2.h"
#include<iostream>
#include<cassert>
#include<fstream>
#include "nav.h"

using namespace std;

Nav2::Nav2::Aturn::Aturn():l(0),r(0),dur(0){}

Nav2::Nav2::Aturn::Aturn(float a,float b,float c):l(a),r(b),dur(c){}

vector<Nav2::NavS> Nav2::loadnav(Navloadinput navin){

	const string MYFILE="/home/lvuser/navlogs/navlog.txt";

	float amount = 0;
	vector<NavS> nav;
	NavS navelement;
	Navinput start, end;
	vector<pair<int,Movedir>> v;
	ofstream myfile(MYFILE);
	//oiload oinav;

	myfile << "hi" << "\n";
	myfile.flush();
	//assign start information
	start.navpt.x = navin.ptone.x;
	start.navpt.y = navin.ptone.y;
	start.navdir = navin.dirone;
	
	//assign end information
	end.navpt.x = navin.pttwo.x;
	end.navpt.y = navin.pttwo.y;
	end.navdir = navin.dirtwo;

	myfile << "startpt: " << start.navpt.x << "," << start.navpt.y << "endpt: " << end.navpt.x << "," << end.navpt.y  << endl;
	v=solvemaze(start.navpt,end.navpt,start.navdir,end.navdir);
	
	myfile << "size: " << v.size() << "\n"; 
	//something to note is that doing a 180 or going back is going to be the same as turning exept that it is going to be for longer so that it can go as far 

	for (unsigned int i=0;i<v.size();i++){
		myfile << "Processing " << i <<  "\n";
		if(v[i].second == MFORWARD){
			navelement.left = -.25;
			navelement.right = -.25;
			amount += v[i].first / 29.0;
		}
		else if(v[i].second == MLEFT){
			navelement.left = .25;
			navelement.right = -.25;
			amount += .65;
		}
		else if(v[i].second == MRIGHT){
			navelement.left= -.25;
			navelement.right= .25;
			amount += .65;
		}
		else if(v[i].second == MBACK){
			navelement.left= .25;
			navelement.right= -.25;
			amount+= 1.1;
		}
		else 
			assert(0);

		navelement.amount = amount;

		myfile << "Pushing " << "navelm.left " << navelement.left << " navelm.right " << navelement.right << " amount " << navelement.amount << endl;
		nav.push_back(navelement);

		// push a delay
		navelement.left= 0;
		navelement.right= 0;
		amount += 1;
		navelement.amount = amount;
		nav.push_back(navelement);

		myfile << "Pushing " << "navelm.left " << navelement.left << " navelm.right " << navelement.right << " amount " << navelement.amount << endl;
	}
	myfile.close();
	return nav;
}

Nav2::Nav2(){
	stepcounter=0;
	//the information that is being declared are just place holders for when we get and actual values for auto.
	s1.ptone.x=146;
	s1.ptone.y=130;
	s1.pttwo.x=33;
s1.pttwo.y=135;
	s1.dirone=LEFT;
	s1.dirtwo=UP;
		
	s2.ptone.x=70;
	s2.ptone.y=70;
	s2.pttwo.x=70;
	s2.pttwo.y=70;
	s2.dirone=LEFT;
	s2.dirtwo=LEFT;

	s3.ptone.x=70;
	s3.ptone.y=70;
	s3.pttwo.x=60;
	s3.pttwo.y=70;
	s3.dirone=LEFT;
	s3.dirtwo=LEFT;
		
	s4.ptone.x=70;
	s4.ptone.y=70;
	s4.pttwo.x=70;
	s4.pttwo.y=60;
	s4.dirone=LEFT;
	s4.dirtwo=LEFT;
		
	s5.ptone.x=0;
	s5.ptone.y=0;
	s5.pttwo.x=79;
	s5.pttwo.y=0;
	s5.dirone=LEFT;
	s5.dirtwo=LEFT;
	
	Aturn={0,0,0};
	
	navindex=0;

	stepcounter = 1;
}

#ifdef NAV2_TEST
int main(){
	return 0;
}
#endif
