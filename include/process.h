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
  int Pid() const;                               
  std::string User() const;                      
  std::string Command() const;                   
  float CpuUtilization() const;                  
  void CpuUtilization(long active_jiffs, long jiffs);
  std::string Ram() const;                       
  long int UpTime() const;                       
  bool operator<(Process const& a) const;  
  bool operator>(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
    int pid_;
    float cpu_{0};
    long prev_active_jiffs{0};
    long prev_jiffs{0};
};

#endif