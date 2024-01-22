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
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();

  // Declaration of the private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  std::string ram_;
  long int up_time_;
  float cpu_utilization_;

    // CPU values of a process
  enum ProcessCPUStates {
    kUtime_ = 0,
    kStime_,
    kCutime_,
    kCstime_,
    kStarttime_
  };
    //sets the Cpu utilization in each process
  void setCpuUtilization();
};

#endif