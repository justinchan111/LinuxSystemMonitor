#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid) : pid_(pid){};

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
  float Active; // in jiffies
  float elapsed_time; // in seconds
  
  Active = LinuxParser::ActiveJiffies(pid_);
  elapsed_time = LinuxParser::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
  
  // Calculate difference between current and old CPU values
  float Active_d = Active - prev_Active;
  float elapsed_time_d = elapsed_time - prev_elapsed_time;
    
  // Calculate CPU utilisation if elapsed_time_d > 1 secs
  if( elapsed_time_d > 1 && Active_d > 0) {
    float CPU_perc;
    CPU_perc = (Active_d / sysconf(_SC_CLK_TCK)) / elapsed_time_d;
    
    // Set current CPU values to previous
    prev_Active = Active;
    prev_elapsed_time = elapsed_time;
    prev_CPU_perc = CPU_perc;
    
    cpuUti = CPU_perc;
    
    return CPU_perc; 
  }
 
  return prev_CPU_perc; 
}


string Process::Command() { 
  if(command == "") {
    command = LinuxParser::Command(pid_);
    if (command.size() > 40) {
      command.erase(command.begin()+40, command.end());
      command.append("...");
    }
  }
  return command; 
}


string Process::Ram() { 
  return LinuxParser::Ram(pid_); 
}

string Process::User() { 
  if(username == "") username = LinuxParser::User(pid_);
  return username; 
}

long int Process::UpTime() { 
  return LinuxParser::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK); 
}

//  Overload "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return a.cpuUti < cpuUti; 
}
