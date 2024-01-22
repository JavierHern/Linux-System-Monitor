#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  void CpuUtilization(long a_ticks, long sys_ticks);
  long Jiffies() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(const Process& a) const;
  bool operator>(const Process& a) const;

  // Declaration of the private members
 private:
  int pid_;
  float cpu_utilization_{0};
  long active_ticks{0};
  long idle_ticks{0};
  long system_ticks{0};
};

#endif