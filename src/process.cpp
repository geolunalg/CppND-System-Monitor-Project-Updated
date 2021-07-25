#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid) {};

// Return this process's ID
int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpu_; }

// Return this process's CPU utilization
void Process::CpuUtilization(long active_jiffs, long jiffs) { 
    long diff_active_jiffs = active_jiffs - prev_active_jiffs;
    long diff_jiffs = jiffs - prev_jiffs;
    cpu_ = static_cast<float>(diff_active_jiffs) / diff_jiffs;
    prev_active_jiffs = active_jiffs;
    prev_jiffs = jiffs;
}

// Return the command that generated this process
string Process::Command() const { 
    return LinuxParser::Command(Pid()); 
}

// Return this process's memory utilization
string Process::Ram() const { 
    return LinuxParser::Ram(Pid()); 
}

// Return the user (name) that generated this process
string Process::User() const { 
    return LinuxParser::User(Pid()); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() const { 
    return LinuxParser::UpTime(Pid()); 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return CpuUtilization() < a.CpuUtilization(); 
}

// Overload the "greater than" comparison operator for Process objects
bool Process::operator>(Process const& a) const { 
    return CpuUtilization() > a.CpuUtilization(); 
}