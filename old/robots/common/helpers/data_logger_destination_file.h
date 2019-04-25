#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

#include "data_logger_destination.h"

using namespace std;


template <typename DATATYPE>
class DataloggerDestinationFile : public DataloggerDestination<DATATYPE> {
private:
	ofstream output;
	bool     report_col_name;

public:
	DataloggerDestinationFile(string filename);
	virtual ~DataloggerDestinationFile();
	
	virtual DataloggerDestinationFile& operator<<(double value);
	virtual DataloggerDestinationFile& operator<<(string value);

	void setReportColName(bool report);
	bool getReportColName() const;
	void endLoop(const vector<string>& columns, const vector<DATATYPE>& data);
};


template <typename DATATYPE>
DataloggerDestinationFile<DATATYPE>::DataloggerDestinationFile(string filename)
{
	output = ofstream(filename);
	report_col_name = true;
}

template <typename DATATYPE>
DataloggerDestinationFile<DATATYPE>::~DataloggerDestinationFile()
{
	output.close();
}

template <typename DATATYPE>
DataloggerDestinationFile<DATATYPE>& DataloggerDestinationFile<DATATYPE>::operator<<(double value)
{
	output << std::to_string(value);
	return *this;

}

template <typename DATATYPE>
DataloggerDestinationFile<DATATYPE>& DataloggerDestinationFile<DATATYPE>::operator<<(string value)
{
	output << value;
	return *this;
}

template <typename DATATYPE>
void DataloggerDestinationFile<DATATYPE>::setReportColName(bool report)
{
	report_col_name = report;
}

template <typename DATATYPE>
bool DataloggerDestinationFile<DATATYPE>::getReportColName() const
{
	return report_col_name;
}

template <typename DATATYPE>
void DataloggerDestinationFile<DATATYPE>::endLoop(const vector<string>& columns, const vector<DATATYPE>& data)
{
	for (size_t i = 0; i < columns.size(); i++)
	{
		if (getReportColName()) {
			*this << columns[i] << "=";
		}
		*this << data[i];

		if (i < columns.size() - 1) {
			*this << "  ";
		}
	}
	*this << "\n";
}
