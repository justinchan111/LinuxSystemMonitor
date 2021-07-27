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
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
 
  
  float cpuUti;
  int ram;
  // TODO: Declare any necessary private members
 private:
  int pid_;
  
  // Previous CPU values for this process
  float prev_Active = 0; // in jiffies
  float prev_elapsed_time = 0; // in seconds
  float prev_CPU_perc = 0;
  std::string command = ""; // cache for command
  std::string username = ""; // cache for username
  
};

#endif