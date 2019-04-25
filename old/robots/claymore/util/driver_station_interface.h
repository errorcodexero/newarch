#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include "maybe.h"

//this is the interface for the OI

struct Driver_station_output{
    static const unsigned DIGITAL_OUTPUTS=6;//Note: this is dependent on the option of the TI Launchpad MSP430 - this is for option 2 from 2017
    std::bitset<DIGITAL_OUTPUTS> digital;

    Driver_station_output();
};

bool operator==(Driver_station_output,Driver_station_output);
bool operator!=(Driver_station_output,Driver_station_output);
std::ostream& operator<<(std::ostream&,Driver_station_output);

struct Driver_station_input{
    static const unsigned ANALOG_INPUTS=8;//TODO get real value
    std::array<double,ANALOG_INPUTS> analog;

    static const unsigned DIGITAL_INPUTS=8;//TODO get real value
    std::bitset<DIGITAL_INPUTS> digital;

    Driver_station_input();

    static Maybe<Driver_station_input> parse(std::string const&);
    static Driver_station_input rand();
};
bool operator<(Driver_station_input,Driver_station_input);
bool operator==(Driver_station_input,Driver_station_input);
bool operator!=(Driver_station_input,Driver_station_input);
std::ostream& operator<<(std::ostream&,Driver_station_input);
