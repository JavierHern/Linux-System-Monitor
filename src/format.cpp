#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    string time = "00:00:00";
    long s = 0;
    if (seconds > 0){
        //calculate the hours
        s = seconds / 3600;
        time = timeToString(s) + ":";
        //calculate the minutes
        s = (seconds / 60) % 60;
        time += timeToString(s) + ":";
        //calculate the seconds
        s = seconds % 60;
        time += timeToString(s);
    }
    return time;
}

string Format::timeToString(long s){
    if(s < 10){
        return "0" + std::to_string(s);
    }else{
        return std::to_string(s);
    }
}