#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float mem_util {0.0};
  float mem_total {0.0};
  float mem_free {0.0};
  string line;
  string key, value;
  std::ifstream fstream(kProcDirectory + kMeminfoFilename);
  if (fstream.is_open()) {
    while(std::getline(fstream, line)){
      std::istringstream lstream(line);
        lstream >> key >> value;
        if(key == "MemTotal:") {
          mem_total = stof(value);
        }
        if(key == "MemFree:") {
          mem_free = stof(value);
        }
        if (mem_total != 0.0 && mem_free != 0.0) {
          break;
        }
    }
  }
  mem_util = (mem_total - mem_free) / mem_total;
  return mem_util; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long up_time{0};
  string line;
  string value {""};
  std::ifstream fstream(kProcDirectory + kUptimeFilename);
  if (fstream.is_open()) {
    std::getline(fstream, line);
    std::istringstream lstream(line);
    lstream >> value;
    if (value != "") {
      up_time = stoi(value);
    }
  }
  return up_time; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long jiffs = UpTime() * sysconf(_SC_CLK_TCK);
  return jiffs; 
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long active_jiffs {0};
  string id = to_string(pid);
  string line;
  string key, value;
  vector<string> cpu_util = {};
  std::ifstream fstream(kProcDirectory + id + kStatFilename);
  if (fstream.is_open()) {
    std::getline(fstream, line);
    std::istringstream lstream(line);
    int i = 1;
    while(lstream >> value) {
      if (i == 14 || i == 15 || i == 16) {
        active_jiffs += stol(value);
      }
      if (i == 17) {
        active_jiffs += stol(value);
        break;
      }
      i++;
    }
  }
  return active_jiffs; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long active_jiffs {0};
  auto cpu_util = LinuxParser::CpuUtilization();
  active_jiffs = stol(cpu_util[CPUStates::kUser_]) + stol(cpu_util[CPUStates::kNice_]) + 
                 stol(cpu_util[CPUStates::kSystem_]) + stol(cpu_util[CPUStates::kIRQ_]) + 
                 stol(cpu_util[CPUStates::kSoftIRQ_]) + stol(cpu_util[CPUStates::kSteal_]);
  return active_jiffs;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idle_jiffs {0};
  auto cpu_util = LinuxParser::CpuUtilization();
  idle_jiffs = stol(cpu_util[CPUStates::kIdle_]) + stol(cpu_util[CPUStates::kIdle_]);
  return idle_jiffs; 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key, value;
  vector<string> cpu_util = {};
  std::ifstream fstream(kProcDirectory + kStatFilename);
  if (fstream.is_open()) {
    while(std::getline(fstream, line)) {
      std::istringstream lstream(line);
      while(lstream >> key) {
        if (key == "cpu") {
          while (lstream >> value) {
            cpu_util.push_back(value);
          }
        }
      }
    }
  }
  return cpu_util; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total_procss{0};
  string key, value;
  string line;
  std::ifstream fstream(kProcDirectory + kStatFilename);
  if (fstream.is_open()) {
    while(std::getline(fstream, line)) {
      std::istringstream lstream(line);
      lstream >> key >> value;
      if (key == "processes") {
        total_procss = stoi(value);
        break;
      }
    }
  }
  return total_procss; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runing_procs {0};
  string key, value;
  string line;
  std::ifstream fstream(kProcDirectory + kStatFilename);
  if (fstream.is_open()) {
    while(std::getline(fstream, line)) {
      std::istringstream lstream(line);
      lstream >> key >> value;
      if (key == "procs_running") {
        runing_procs = stoi(value);
        break;
      }
    }
  }
  return runing_procs; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command {""};
  string id = to_string(pid);
  string line;
  std::ifstream fstream(kProcDirectory + id + kCmdlineFilename);
  if (fstream.is_open()) {
    std::getline(fstream, line);
    if (line != "") {
      command = line;
    }
  }
  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  long ram{0};
  string id = to_string(pid);
  string line;
  string key, value;
  std::ifstream fstream(kProcDirectory + id + kStatusFilename);
  if (fstream.is_open()) {
    while(std::getline(fstream, line)) {
      std::stringstream lstream(line);
      lstream >> key >> value;
      if (key == "VmSize:") {
        ram = stol(value) / 1024;
        break;
      }
    }
  }
  return to_string(ram); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string uid{""};
  string id = to_string(pid);
  string line;
  string key, value;
  std::ifstream fstream(kProcDirectory + id + kStatusFilename);
  if (fstream.is_open()) {
    while(std::getline(fstream, line)){
      std::istringstream lstream(line);
      lstream >> key >> value;
      if (key == "Uid:") {
        uid = value;
        break;
      }
    }
  }
  return uid; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string user {""};
  string line;
  string key = ":x:" + Uid(pid);
  std::ifstream fstream(kPasswordPath);
  if (fstream.is_open()) {
    while(std::getline(fstream, line)) {
      std::size_t found = line.find(key);
      if (found != string::npos)  {
        user = line.substr(0, found);
      }
    }
  }
  return user; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long up_time {0};
  string id = to_string(pid);
  string line;
  string value;
  std::ifstream fstream(kProcDirectory + id + kStatFilename);
  if (fstream.is_open()) {
    std::getline(fstream, line);
    std::istringstream lstream(line);
    int i = 1;
    while(lstream >> value) {
      if(i == 22) {
        up_time = UpTime() - stol(value) / sysconf(_SC_CLK_TCK);
        break;
      }
      i++;
    }
  }
  return up_time; 
}
