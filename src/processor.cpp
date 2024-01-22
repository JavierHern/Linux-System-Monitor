#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  float utilization = 0.0;
  // Reads the values from the Jiffies fucions
  long a_ticks = LinuxParser::ActiveJiffies();
  long i_ticks = LinuxParser::IdleJiffies();
  // Calculates the total time utilized since the boot
  long d_active = a_ticks - active_ticks;
  long d_idle = i_ticks - idle_ticks;
  long d_total = d_active + d_idle;
  // Calculates the utilization
  utilization = static_cast<float>(d_active) / d_total;
  // Sets the previous value iqual to the actual
  active_ticks = a_ticks;
  idle_ticks = i_ticks;

  return utilization;
}