#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) : pid_(pid){};

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// Previous CPU values for this process
float prev_Active = 0; // in jiffies
float prev_elapsed_time = 0; // in seconds
float prev_CPU_perc = 0;

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  float Active; // in jiffies
  float elapsed_time; // in seconds
  
  Active = LinuxParser::ActiveJiffies(pid_);
  elapsed_time = LinuxParser::UpTime() - LinuxParser::UpTime() / CLOCKS_PER_SEC;
  
  // calculate difference between current and old CPU values
  float Active_d = Active - prev_Active;
  float elapsed_time_d = elapsed_time - prev_elapsed_time;
    
  // Calculate CPU utilisation if elapsed_time_d > 0
  if( elapsed_time_d > 0) {
    float CPU_perc;
    CPU_perc = (Active_d / CLOCKS_PER_SEC) / elapsed_time_d;
    
    // Set current CPU values to previous
    prev_Active = Active;
    prev_elapsed_time = elapsed_time;
    prev_CPU_perc = CPU_perc;
    
    cpuUti = CPU_perc;
    
    return CPU_perc; 
  }
 
  return prev_CPU_perc; 
}

string command = ""; // cache for command

// TODO: Return the command that generated this process
string Process::Command() { 
  if(command == "") command = LinuxParser::Command(pid_);
  return command; 
}


// TODO: Return this process's memory utilization
string Process::Ram() { 
  ram = std::stoi(LinuxParser::Ram(pid_));
  return to_string(ram); 
}

string username = ""; // cache for username

// TODO: Return the user (name) that generated this process
string Process::User() { 
  if(username == "") username = LinuxParser::User(pid_);
  return username; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return (cpuUti < a.cpuUti) ? true : false; 
}