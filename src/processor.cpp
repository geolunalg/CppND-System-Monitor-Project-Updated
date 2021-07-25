#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    float utilization {0.0};
    long idle = LinuxParser::IdleJiffies();
    long non_idle = LinuxParser::ActiveJiffies();
    long total = idle + non_idle;
    long total_diff = total - prev_total;
    long idle_diff = idle - prev_idle;
    utilization = static_cast<float>(total_diff - idle_diff) / total_diff;
    prev_total = total;
    prev_idle = idle;
    return utilization; 
}