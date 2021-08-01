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
  bool operator<(Process const& a) const;  
 
  float cpuUti;
  int ram;
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
