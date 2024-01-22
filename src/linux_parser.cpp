#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read and return the Operating System name
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the Kernel's System
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Read and return the Process Identifiers
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the total used memory of the system
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float memTotal = 1;
  float memFree = 0.0;
  float buffer = 0.0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      //Removes all the spaces, and the kB. Replace the : for one space
      //std::remove(line.begin(), line.end(), "kB");
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if ( key == "MemTotal"){
          //stof transform value from string to float
          memTotal = stof(value);
        }
        else if (key == "MemFree"){
          memFree = stof(value);
        }
        else if (key ==  "Buffers"){
          buffer = stof(value);
        }
      }
    }
  }
  return 1 - memFree / (memTotal - buffer); 
}


// Read and return the system UpTime
long LinuxParser::UpTime() { 
  string line, uos;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uos;
  }
  return std::stol(uos);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, key;
  vector<string> vals;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key){
      vals.push_back(key);
    }
  }
  long jiff = 0;
  if (vals.size() > 21){
    long user = stol(vals[13]);
    long kernel = stol(vals[14]);
    long cuser = stol(vals[15]);
    long ckernel = stol(vals[16]);
    jiff = user + kernel + cuser + ckernel;
  }
  return jiff;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[kUser_]) + stol(time[kNice_]) + stol(time[kSystem_]) + stol(time[kIRQ_]) +
          stol(time[kSoftIRQ_]) + stol(time[kSteal_]) + stol(time[kGuest_])+ stol(time[kGuestNice_]));
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[kIdle_]) + stol(time[kIOwait_]));
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key;
  vector<string> timeValues{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key ){
        if(key == "cpu"){
          while(linestream >> key){
            timeValues.push_back(key);
          }
        }
      }
    }
  }
   return timeValues;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes"){
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value); 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {  
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running"){
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline (stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "Uid"){
          return value;
        }
      }
    }
  }
  return value; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, user, key, value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> user >> key >> value){
        if (value == uid){
          return user;
        }
      }
    }
  }
  return user;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid)+ kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "VmSize"){
          return std::to_string(std::stof(value)/1024);
        }
      }
    }
  }
  return string("0");
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  long up_time = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    for (int i = 0; stream >> value; i++){
      // read the start time value in clock ticks and convert it to seconds
      if (i == 13){
        up_time = std::stol(value) / sysconf(_SC_CLK_TCK);
        return up_time;
      }
    }
  }
  return up_time;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line{};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline (stream, line);
    return line;
  }
  return line; 
}
