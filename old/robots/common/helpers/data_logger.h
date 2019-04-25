#pragma once

using namespace std;
#include <vector>
#include <iostream>
#include <string>
#include <assert.h>

#include "data_logger_destination.h"

template <typename DATATYPE>
class Datalogger {
private:
	vector<string> columns;
	vector<DATATYPE> data;
	vector<bool> data_entered;
	vector< DataloggerDestination<DATATYPE>* > destinations;

public:

	void addDestination(DataloggerDestination<DATATYPE>* new_destination);
	int createColumn(string name);
	void startLoop();
	void logData(int colnumber, DATATYPE value);
	void endLoop();
};


template<typename DATATYPE>
void Datalogger<DATATYPE>::addDestination(DataloggerDestination<DATATYPE>* new_destination)
{
	destinations.push_back(new_destination);
}

template<typename DATATYPE>
int Datalogger<DATATYPE>::createColumn(string name)
{
	columns.push_back(name);
	return columns.size() - 1;
}

template<typename DATATYPE>
void Datalogger<DATATYPE>::startLoop()
{
	data.resize(columns.size());
	data_entered.resize(columns.size());

	data_entered.assign(data_entered.size(), false);
	data.assign(data.size(), DATATYPE());

}

template<typename DATATYPE>
void Datalogger<DATATYPE>::logData(int colnumber, DATATYPE value)
{
	// Ensure the same column is not being written to more than once per iteration
	assert(!data_entered[colnumber]);

	data[colnumber] = value;
	data_entered[colnumber] = true;
}

template<typename DATATYPE>
void Datalogger<DATATYPE>::endLoop()
{
	// At least one destinations must have been added
	assert(!destinations.empty());

	for (auto iter(destinations.begin()); iter != destinations.end(); ++iter) {
		(*iter)->endLoop(columns, data);
	}
}