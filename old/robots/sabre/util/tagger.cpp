#include <iostream>
#include <fstream>
#include "tagger.h"
#include "interface.h"

using namespace std;

/* tagThis()
	needs a sting to put into the log file
	needs macro "__FILE__" to mark location of method call

	Note: Think of this like a sticky note. 
		If a usb is plugged into the roboRIO, a list is created on it
		that contains all the notes in the robot, in order called.

	Note: CSV decoder found in fiddle can translate the info in the
		log file.
*/
void tagThis(string str, string str2){
	//True = logs only when enabled (Preffered when going into matches)
	//False = logs while robot is on (Warning: can create very large files over long periods of time!)
	bool LOG_WHILE_ENABLED = true; 
	bool TAGGER_ENABLED = true;
	
	fstream myfile("/media/sda1/taglog.csv", ofstream::out | ofstream::app);
	if(myfile.bad()) return;
	//if(myfile.good()) 
		Robot_inputs robo_time;
		Robot_mode modes;
		//bool robot_enabled = modes.enabled;
		//double robot_time = time.now;
		//double my_time;
		
		
		if(TAGGER_ENABLED && modes.enabled & LOG_WHILE_ENABLED) {
			myfile << "---------------------," << robo_time.now;
			str2.erase(str2.size());
			str2.append(",");
			str2.append(str);
			myfile << str2<< endl;
		}
		else if(TAGGER_ENABLED && !LOG_WHILE_ENABLED) {
			myfile << "---------------------," << robo_time.now;
			str2.erase(str2.size());
			str2.append(",");
			str2.append(str);
			myfile << str2<< endl;
		}
	myfile.close();
}

//Clock idea: interface.h Robot_inputs::now
//interface.h Robot_mode::enabled
