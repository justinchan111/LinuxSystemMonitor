#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm> 

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
  cpu_.Utilization();
  return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  for(const int & pid : pids) {
    processes_.emplace_back(pid);
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_; 
}

string kernel = ""; // cache for system's kernel identifier

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  if (kernel == "") kernel = LinuxParser::Kernel(); // store kernel identifier into cache
  return kernel; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

string osname = ""; // cache for operating system name

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  if (osname == "") osname = LinuxParser::OperatingSystem(); // store OS name into cache
  return osname; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime();
}