#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
   Processor();
  float Utilization();

  // The private members are the previous values and a method for converting vectors
 private:
  float prevIdleCpuTime;
  float prevTotalCpuTime;
  std::vector<long> convertToLong(std::vector<std::string> values);
};

#endif