#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();

    // create a set track process
    set<int> process_pids;
    for (Process const& process : processes_) {
        process_pids.insert(process.Pid());
    }

    // emplaces pids on process object
    for (int pid : pids) {
    if (process_pids.find(pid) == process_pids.end())
        processes_.emplace_back(pid);
    }

    // update the processes cpu utilization
    for (auto& process : processes_) {
        process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()), LinuxParser::Jiffies());
    }

    // sort the process
    std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
    return processes_;
}

// Return the system's kernel identifier
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}