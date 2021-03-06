#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read operating system
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read kernel
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Get process IDs
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key;
  float memTotal, memFree, value;
  string unit;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") memTotal = value;
        if (key == "MemFree") {
          memFree = value;
          return (memTotal - memFree) / memTotal; 
        }
      }
    }
  }
  return 0;
}

// Read the system uptime
long LinuxParser::UpTime() { 
  float uptime = 0;
  float idletime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime; 
}

// Read the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string key;
  long int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
  }
  return 0; 
}

// Read the number of active jiffies for a Process ID
long LinuxParser::ActiveJiffies(int pid) { 
  string value;
  long int utime, stime;
  string line;
  std::ifstream stream (kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 1; i < 18; i++) {
      linestream >> value;
      if (i == 14) utime = std::stol(value);
      else if (i == 15) stime = std::stol(value);
    }
    return utime + stime;
  }
  return 0; 
}

// Read the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string key;
  long int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    return user + nice + system + irq + softirq + steal;
  }
  return 0; 
}

// Read the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string key;
  long int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    return idle + iowait;
  }
  return 0; 
}

// Previous CPU jiffy values
double prev_Idle, prev_Total = 0;
double prevCPU_perc = 0;

// Read CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  double Idle, Active, Total;
  Idle = LinuxParser::IdleJiffies();
  Active = LinuxParser::ActiveJiffies();
  Total = Idle + Active;
  // Calculate difference between current and old jiffies
  double Total_d = Total - prev_Total;
  double Idle_d = Idle - prev_Idle;
  
  // Calculate CPU utilisation if Total_d > 0;
  if (Total_d > 0){
    double CPU_perc;
    CPU_perc = (Total_d - Idle_d) / Total_d;
    prevCPU_perc = CPU_perc;
    // Set current jiffies to previous jiffies
    prev_Idle = Idle;
    prev_Total = Total;
    return vector<string> {to_string(CPU_perc)};
  }
  // return previous cpu utilisation if no changes occur
  return vector<string> {to_string(prevCPU_perc)}; 
}

// Read the total number of processes
int LinuxParser::TotalProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return 0; 
}

// Read the number of running processes
int LinuxParser::RunningProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return 0; 
}

// Read the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream (kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, command);
    return command;
  }
  return string(); 
}

// Read the memory used by a process
string LinuxParser::Ram(int pid) { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmRSS") { // VmRss is the exact physical memory being used as a part of Physical RAM
        long temp = std::stol(value);
        temp /= 1000;
        return to_string(temp);
      }
    }
  }
  return string(); 
}

// Read the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid") return value;
    }
  }
  return string(); 
}

// Read the user associated with a process
string LinuxParser::User(int pid) { 
  string user, x, uid_desired, uid;
  string line;
  uid_desired = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
      if(uid == uid_desired) return user;
    }  
  }
  return string(); 
}

// Read the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string value;
  string line;
  std::ifstream stream (kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 1; i < 23; i++) {
      linestream >> value;
      if (i == 22) return std::stol(value);
    }
  }
  return 0; 
}
