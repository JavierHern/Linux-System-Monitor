#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Inicialize all values
Process::Process(int pid) : 
    pid_(pid), 
    user_(LinuxParser::User(Pid())),
    command_(LinuxParser::Command(Pid())),
    ram_(LinuxParser::Ram(Pid())),
    up_time_(LinuxParser::UpTime(Pid()))
    {
        setCpuUtilization();
    }

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilization_; }

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }


void Process::setCpuUtilization(){
    // read values from filesystem
    long uptime = LinuxParser::UpTime();
    vector<float> val = LinuxParser::CpuUtilization(Pid());
    // only if the values could be read sucessfully
    if (val.size() == 5) {
        // add utime, stime, cutime, cstime (they are in seconds)
        float totaltime =
            val[kUtime_] + val[kStime_] + val[kCutime_] + val[kCstime_];
        float seconds = uptime - val[kStarttime_];
        // calculate the processes CPU usage
        cpu_utilization_ = totaltime / seconds;
    } else
        cpu_utilization_ = 0;
}