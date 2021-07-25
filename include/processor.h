#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

 private:
 long prev_idle {0};
 long prev_total {0}; 
};

#endif