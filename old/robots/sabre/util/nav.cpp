//something that  did not look right was that even though the start was at the far right and start orentation was right it when strait even thought there should be a wall there.
#include "nav.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <random>
#include <time.h> 
#include <assert.h>
#include <unistd.h>

#define MAPWIDTH 160
#define MAPLENGTH 162//add.5

#define STARTONE 70
#define STARTTWO 110
#define ENDONE 150
#define ENDTWO 110
//#define NAV_TEST

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Structures
//
// All Structures
/////////////////////////////////////////////////////////////////////////////

struct Mapstruct {
	int width;
	int length;
	bool walls[MAPWIDTH][MAPLENGTH];
};

struct List{
	Pt2 pt;
	Pt2 prev;
	bool v;
};
/////////////////////////////////////////////////////////////////////////////
// operatior
//
// allow the use of diffrent things with the structure point
/////////////////////////////////////////////////////////////////////////////
bool operator!=(Pt2 a,Pt2 b){
	return (a.x != b.x || a.y != b.y);
}

ostream&operator<<(ostream& o, Pt2 a){
	o<< a.x << "," << a.y << endl;
	return o;
}

bool operator==(Pt2 a,Pt2 b){
	if ((a.x == b.x) & (a.y == b.y))return true;
	return false;
}

template<typename A, typename B>
ostream&operator<<(ostream& o, pair<A,B> const& p){
	o << "Pairs: " << p.first << "," << p.second << endl; 
	return o;
}
template<typename A, typename B>
ostream&operator<<(ostream& o, vector<A,B> const& p){
	for (unsigned int i=0; i < p.size();i++)o << "v: " << p[i] << endl; 
	return o;
}

ostream&operator<<(ostream& o, Movedir const& p){
	if(p==MFORWARD)o<< "Forward";
	else if( p==MRIGHT)o<< "Right";
	else if (p==MLEFT)o<<"Left";
	else if (p==MBACK)o<<"Back"; 
	return o;
}
//////////////////////////////////////////////////////////////////////////////
//pbool
//
//Takes in a boolian variable and returns a string that is ether TRUE or FALSE
//////////////////////////////////////////////////////////////////////////////
string pbool(bool b){
	if(b) return "true";
	return "false";
}
///////////////////////////////////////////////////////////////////////////////////////////
//pmovedir
//
//takes in a enum variable and returns a string that is the equivilent to the enum's values
///////////////////////////////////////////////////////////////////////////////////////////
string pmovedir(Movedir a){
	string b;
	if(a == MLEFT) return "Left";
	else if(a == MRIGHT) return "Right";
	else if(a == MFORWARD) return "Forward";
	else if(a == MBACK) return "Back";
	return "nothing was entered";
}
	
/////////////////////////////////////////////////////////////////////////////
// bounderies
//
// returns true if p is in the grid
/////////////////////////////////////////////////////////////////////////////
bool bounderies(Pt2 p,Mapstruct const& map){
	bool xvalid = p.x >=0+ROBOTSPACE && p.x < map.width;
	bool yvalid = p.y >=0+ROBOTSPACE && p.y < map.length;
	return xvalid && yvalid;
}

//////////////////////////////////////////////////////////////////////////////
// Walls
//
// returns true if p is not a wall
//////////////////////////////////////////////////////////////////////////////
bool Walls(Pt2 p,Mapstruct const& map){
	return !map.walls[p.x][p.y];	
}
/////////////////////////////////////////////////////////////////////////////
// valid																   
//																		   
// returns true if p is not a wall or boundry                              
/////////////////////////////////////////////////////////////////////////////
bool valid(Pt2 p,Mapstruct  const& map){
	return (bounderies(p,map) && Walls(p,map));
}

/////////////////////////////////////////////////////////////////////////////
// validpoint
//
// returns true if the given point is not a wall or boundry and has not been visited
/////////////////////////////////////////////////////////////////////////////
bool validpoint(vector<List>  const& v,Pt2 q,Mapstruct const& map){
	int i;
	int max;
	bool p ;
	
	p=false;
	max = v.size();

	for(i=0; i<max; i++){
		if(v[i].pt == q){
			p = true;
			break;
		}
	}
		
	return valid(q,map) && !p;
}

/////////////////////////////////////////////////////////////////////////////
// [dir]p
//
// returns adjacent points
/////////////////////////////////////////////////////////////////////////////
Pt2 leftp(Pt2 p){
	return Pt2{p.x - 1,p.y};
}

Pt2 rightp(Pt2 p){
	return Pt2{p.x + 1,p.y};
}

Pt2 upp(Pt2 p){
	return Pt2{p.x,p.y - 1};
}

Pt2 downp(Pt2 p){
	return Pt2{p.x,p.y + 1};
}
/////////////////////////////////////////////////////////////////////////////
// nextp
//

//returns next point that has not been visited
////////////////////////////////////////////////////////////////////////////
int nextp(vector<List> const& v){
	int max;
	int i;

	max = v.size();

	for (i=0; i<max; i++){
		if( !v[i].v)break;
	}

	assert(i<max);
	return i;
}
/////////////////////////////////////////////////////////////////////////////
// getpoint
//
// given a point will return a vector of points that are valid
/////////////////////////////////////////////////////////////////////////////
vector<Pt2> getpoint(vector<List> const& v,Pt2 p,Mapstruct const& map){
	Pt2 a;
	Pt2 b;
	Pt2 c;
	Pt2 d;

	vector<Pt2> validpoints;
	
	a = leftp(p);
	b = rightp(p);
	c = upp(p);
	d = downp(p);

	if(validpoint(v,a,map)){
		validpoints.push_back(a);		
	}

	if(validpoint(v,b,map)){
		validpoints.push_back(b);		
	}

	if(validpoint(v,c,map)){

		validpoints.push_back(c);		
	}

	if(validpoint(v,d,map)){
		validpoints.push_back(d);		
	}
	
	return validpoints;

}

/////////////////////////////////////////////////////////////////////////////
//Find Pt2
//
//finds point
////////////////////////////////////////////////////////////////////////////
int findpoint(vector<List> const& v,Pt2 q){
	int max;
	int i;
	max = v.size();
	for (i=0; i<max; i++){
		if( v[i].pt == q)break;
	}
	assert(i<max);
	return i;
}
/////////////////////////////////////////////////////////////////////////////
//getline
//
//request for ints and then input them into a string
////////////////////////////////////////////////////////////////////////////
int  getnum(){ 
	string s;
	
	getline(cin,s);
	return atoi(s.c_str());
	
}
/////////////////////////////////////////////////////////////////////////////
//loadmap
//
//loads an array that is what the program navigates
////////////////////////////////////////////////////////////////////////////
void loadmap(Mapstruct & a){
	a.width = MAPWIDTH;
	a.length = MAPLENGTH;

	for(int i=0; i < a.width; i++){
		for(int j=0; j < a.length; j++){
			a.walls[i][j] = false;
		}
	}
	 //blocks off secret passage.
	for(int i=0; i < 144 + ROBOTSPACE; i++){
		for(int j=0; j < 30+ROBOTSPACE; j++){
			a.walls[i][j] = true;
		}
	}
	//blocks off outerworks.
	for(int i=95-ROBOTSPACE; i < 120.5+ROBOTSPACE; i++){
		for(int j=30.5; j < a.length; j++){
			a.walls[i][j] = true;
		}
	}
	//unblocks off lowbar.
	for(int i=95-ROBOTSPACE; i < 120.5+ROBOTSPACE; i++){
		for(int j=a.length-9-46+ROBOTSPACE; j < a.length-ROBOTSPACE-10; j++){
			a.walls[i][j] = false;
		}
	}

	// blocks of tower.
	for(int i=0; i < 34.5+ROBOTSPACE; i++){
		for(int j=46.5-ROBOTSPACE; j < 119+ROBOTSPACE; j++){
			a.walls[i][j] = true;
		}
	}
	
}
/////////////////////////////////////////////////////////////////////////////////////////////
//whatdir
//
//takes in two points and determines dirction reletive to the first point and returns an enum
/////////////////////////////////////////////////////////////////////////////////////////////
Direction whatdir(Pt2 a,Pt2 b){
	if (a.x > b.x)return LEFT;
	else if (a.x < b.x)return RIGHT;
	else if (a.y > b.y)return UP;
	else if (a.y < b.y)return DOWN;
	assert(0);
	
}
/////////////////////////////////////////////////////////////////////////////
//whatmove
//
//takes in two directions and then finds orentation based on that
////////////////////////////////////////////////////////////////////////////
Movedir whatmove(Direction one, Direction two){
	if(one == two)	return MFORWARD;
	else if(one == RIGHT){
		if(two == UP)return MLEFT;
		else if (two == DOWN)return MRIGHT;
		else if (two == LEFT)return MBACK;
	}
	else if(one == LEFT){
		if(two == UP)return MRIGHT;
		else if(two == DOWN)return MLEFT;
		else if(two == RIGHT)return MBACK;
	}
	else if(one == UP){
		if (two == RIGHT)
			return MRIGHT;
		else if(two == LEFT)
			return MLEFT;
		else if(two == DOWN)
			return MBACK;
	}
	else if(one == DOWN){
		if(two == RIGHT)
			return MLEFT;
		else if(two == LEFT)
			return MRIGHT;
		else if(two == UP)
			return MBACK;
	}

	assert(0);
}
/////////////////////////////////////////////////////////////////////////////
//invertvector
//
//takes in a vector and reverses its order
////////////////////////////////////////////////////////////////////////////
vector<Pt2> invertvector(vector<Pt2> v){
	vector<Pt2> nv;
	for(int i = v.size() - 1; i > -1;i--)
		nv.push_back(v[i]);
	return nv;
}
/////////////////////////////////////////////////////////////////////////////
//findlist
//
//finds common orentations in secsession.
////////////////////////////////////////////////////////////////////////////
vector<pair<int,Direction>> findlist(vector<Direction> v){
	vector<pair<int,Direction>> pairs;
	pair<int,Direction> p;
	

	for(unsigned int i=0; i < v.size(); i++){//loops throught the vector.
		Direction FCE;
		FCE = v[i];
		
		p.first=0;
		p.second = FCE;
		while(i < v.size() && v[i]==FCE){//finds common positions in secsession		
			p.first++;
			i++;
		}
		i--; //the for loop incerments too far so you need to go back to account for all of the elements
		pairs.push_back(p);
	}

	return pairs;
}
/////////////////////////////////////////////////////////////////////////////
//findlist2
//
//finds common relative orentations in secsession.
////////////////////////////////////////////////////////////////////////////
vector<pair<int,Movedir>> findlist2(Direction crdir,vector<pair<int,Direction>> v,Direction enddir){
	vector<pair<int,Movedir>> pairs;
	pair<int,Movedir> p;
	Direction LCD = crdir;
	Movedir turn;

	for(unsigned int i=0; i < v.size(); i++){//loops throught the vector.
		turn = whatmove(LCD,v[i].second);//figures out orentation

		if (turn != MFORWARD){
			p.first = 1;
			p.second = turn;
			pairs.push_back(p);
			LCD = v[i].second;	
		}
		p.first = v[i].first;
		p.second = MFORWARD;
		pairs.push_back(p);
	}
	if (LCD != enddir){
		p.first = 1;
		p.second = whatmove(LCD,enddir);
		pairs.push_back(p);
	}
	
	return pairs;
}

vector<pair<int,Movedir>> solvemaze(Pt2 start,Pt2 end,Direction startdir,Direction enddir){
	//declarations	
	Pt2 p;
	Pt2 e;
	Pt2 f;
	List log;
	Mapstruct map;	
	Direction nextdir;
	vector<List> info;
	vector<Pt2> nextpoint;
	vector<Pt2> path;
	vector<Pt2> ipath;
	vector<Movedir> drivec;
	vector<Movedir> fdrive;
	vector<pair<int,Direction>> pear;
	vector<pair<int,Movedir>> pear2;
	vector<Movedir> finnalout;
	vector<Direction> vnextdir;
	int lastline;
	int lineofvector;
	bool endpoint;
	

	loadmap(map);//loads the map the the robot will navigate

	endpoint = false;
	
	//printing out values before assigning them to points
	
	f.x = start.x - 1;//set
	f.y = start.y - 1;//inital location for previus

	p.x = start.x;//set px
	p.y = start.y;//location of start point py

	e.x = end.x;//set ex
	e.y = end.y;//location of end point ey

	//cout << "INITAL POINTS" << endl;
	//cout <<"prev inital:"<< f << "point inital:" << p << endl;
	//cout << "-----------------------------------------------------------------------" << endl;
	log.prev = f; //sets a prev point 
	log.pt = p; //set the original point
	log.v = false; // set the visited to not visited
	info.push_back(log);//Push the log to info 
		
	while (!endpoint){
			
		lineofvector = nextp(info); //find the line of stored data that is next in line

		p = info[lineofvector].pt;//save that loacation that you want to go to going to
			
		nextpoint = getpoint(info,p,map); // set a vector to have all posable points that are legal
			
		for (unsigned int i = 0; i < nextpoint.size(); i++){         //loop tell for the size of the vector nextpoint
				
			log.prev = p; //store prev point
				
			log.v = false; //set the visisted to not visited
				
			log.pt = nextpoint[i]; // the point that you are storing
				
			if(log.pt == e){
				endpoint = true; // set whether you have compleated the calculations to true
			}

			info.push_back(log); // store all the stored point
		}	
				
		info[lineofvector].v = true; // set the visited status to visited
			
	}
	//cout << "-----------------------------------------------------------------------" << endl;

	//cout << "PATH:" << endl << endl;

	lastline = findpoint(info,e);
	//cout << lastline << "last line" << endl;
	while(lastline != 0){
		//cout << info[lastline].prev << endl;
		path.push_back(info[lastline].pt);
		lastline = findpoint(info,info[lastline].prev);
		//cout << lastline << "saved vector" << endl;
	}

	ipath = invertvector(path); ////invert path

	//for (unsigned int i=0; i<ipath.size(); i++)	
		//cout << ">>" << ipath[i] << endl;

	for(unsigned int i = 0; i < ipath.size()-1;i++){ //loop through the vector and deturmine the route 
		nextdir = whatdir(ipath[i],ipath[i+1]); //figures out direction
		//cout << nextdir << endl;
		vnextdir.push_back(nextdir);
	}
	
	pear = findlist(vnextdir);
	pear2 = findlist2(startdir,pear,enddir);
		
	return pear2;	
}

Pt2 randpoint(Mapstruct map){
	Pt2 a;
	
	while (true){
		a.x = rand() % 119;
		a.y = rand() % MAPLENGTH-1;
		if(valid(a,map))return a;
	}
}

#ifdef NAV_TEST

int main(){
/*	point a;
	point b;
	direction c=RIGHT;
	direction  d=UP;
	vector<pair<int,movedir>> FinalInstructions;
	mapstruct map;

	loadmap(map);
	
	a.x=STARTONE;
	a.y=STARTTWO;
	b.x=ENDONE;
	b.y=ENDTWO;

	//for(int i=1;i<10;i++){
	//a = randpoint(map);
	//b = randpoint(map);
	//cout << "point :" << a << endl << "POINT 2:" << b << endl;

	FinalInstructions = solvemaze(a,b,c,d);

	cout << "Final:" << endl << FinalInstructions << endl;

	//}*/
}

#endif
