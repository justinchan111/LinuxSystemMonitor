#include <string>

#include "format.h"

using std::string;
using std::to_string;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours, minutes, seconds_remainder;
  string elapsed_time;
  hours = seconds / 3600;
  seconds_remainder = seconds - hours * 3600;
  minutes = seconds_remainder / 60;
  seconds_remainder = seconds_remainder - minutes * 60;
  
  string s_hours, s_minutes, s_seconds_remainder;
  
  if(hours < 10) s_hours = "0" + to_string(hours);
  else s_hours = to_string(hours);
  
  if(minutes < 10) s_minutes = "0" + to_string(minutes);
  else s_minutes = to_string(minutes);
  
  if(seconds_remainder < 10) s_seconds_remainder = "0" + to_string(seconds_remainder);
  else s_seconds_remainder = to_string(seconds_remainder);
  
  elapsed_time = s_hours + ":" + s_minutes + ":" + s_seconds_remainder;
  return elapsed_time; 
}
