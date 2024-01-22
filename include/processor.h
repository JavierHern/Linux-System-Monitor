#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();

  // The private members are the previous values and a method for converting vectors
 private:
  long active_ticks{0};
  long idle_ticks{0};
};

#endif