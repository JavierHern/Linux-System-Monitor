#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <iomanip>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long value_in_sec){
    double hours = (double) value_in_sec / 3600.0;
    double hours_int;
    double hours_dec = modf(hours, &hours_int);

    double minutes = hours_dec * 60.0;
    double minutes_int;
    double minutes_dec = modf(minutes, &minutes_int);

    double seconds = minutes_dec * 60.0;
    double seconds_int;
    modf(seconds, &seconds_int);

    std::stringstream stringLine;
    stringLine << std::setfill('0') << std::setw(2) << hours_int << ":" << std::setfill('0') << std::setw(2) << minutes_int << ":" << std::setfill('0') << std::setw(2) << seconds_int;
    return stringLine.str();
}