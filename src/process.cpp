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
Process::Process(int pid) : pid_(pid) {}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_utilization_; }

// Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(Pid()); }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(Pid()); }


void Process::CpuUtilization(long a_ticks, long sys_ticks){
  long act_duraction = a_ticks - active_ticks;
  long total_duration = sys_ticks - system_ticks;
  cpu_utilization_ = static_cast<float>(act_duraction) / total_duration;
  active_ticks = a_ticks;
  system_ticks = sys_ticks;
}

bool Process::operator<(const Process& a) const{
    return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(const Process& a) const{
    return CpuUtilization() > a.CpuUtilization();
}