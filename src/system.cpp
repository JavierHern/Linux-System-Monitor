#include <unistd.h>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::map;
using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::sort;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
// This code helper I saw it from the GitHub repo of Udacity students branch, and I took the inspiration:
// https://github.com/udacity/CppND-System-Monitor-Project-Updated/tree/solution
vector<Process>& System::Processes(){
  vector<int> processes_ids = LinuxParser::Pids();
  set<int> existing_processes;

  auto i = processes_.begin();
  while (i != processes_.end()){
    existing_processes.insert(i->Pid());
    i++;
  }

  auto j = processes_ids.begin();
  while (j != processes_ids.end()){
    int pd = *j;
    if (existing_processes.find(pd) == existing_processes.end()){
      processes_.emplace_back(pd);
    }
    j++;
  }

  auto k = processes_.begin();
  while (k != processes_.end()){
    Process& process = *k;
    process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()), LinuxParser::Jiffies());
    k++;
  }

  sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_;
}
// Return the system's kernel identifier as a string
std::string System::Kernel() const { return LinuxParser::Kernel(); }

// Return the system's memory utilization as a float
float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

// Return the operating system name as a string
std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system as an integer
int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system as an integer
int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running as a long
long int System::UpTime() const { return LinuxParser::UpTime(); }