#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

// Define the constructor eith the previous values in zeros
Processor::Processor() : prevIdleCpuTime(0.0), prevTotalCpuTime(0.0){};

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // Reads the values from the file system and convert the string vector in to a long vector
  vector<long> timeValues = convertToLong(LinuxParser::CpuUtilization());
  // Calculates the total time utilized since the boot
  float totalCpuTime = 
    timeValues[LinuxParser::kUser_] + timeValues[LinuxParser::kNice_] +
    timeValues[LinuxParser::kSystem_] + timeValues[LinuxParser::kIdle_] +
    timeValues[LinuxParser::kIOwait_] + timeValues[LinuxParser::kIRQ_] +
    timeValues[LinuxParser::kSoftIRQ_] + timeValues[LinuxParser::kSteal_] +
    timeValues[LinuxParser::kGuest_] + timeValues[LinuxParser::kGuestNice_];
  
  // Calculates the idle time since boot
  float idleCpuTime = 
    timeValues[LinuxParser::kIdle_] + timeValues[LinuxParser::kIOwait_];

  // Calculate the Cpu usage since the last update
  float totald = totalCpuTime - prevTotalCpuTime;
  float idled = idleCpuTime - prevIdleCpuTime;
  float CPU_Percentage = (totald - idled) / totald;

  prevTotalCpuTime = totalCpuTime;
  prevIdleCpuTime = idleCpuTime;

  return CPU_Percentage;
}

// Converts the strings vector to a longs vector
vector<long> Processor::convertToLong(vector<string> values){
  vector<long> convValues{};
  for (int i = 0; i < (int)values.size(); i++) {
    try {
      convValues.push_back(std::stol(values[i]));
    } catch (const std::invalid_argument& arg) {
      convValues.push_back((long)0);
    }
  }
  return convValues;
}