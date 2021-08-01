#include <string>
#include <vector>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::vector<std::string> cpu_uti = LinuxParser::CpuUtilization();
  return std::stof(cpu_uti[0]);; 
}
