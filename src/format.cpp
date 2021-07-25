#include <string>

#include "format.h"

using std::string;

// return format time in HH:MM:SS format
string Format::ElapsedTime(long seconds) { 
    string hh = std::to_string(seconds / (60 * 60));
    string mm =std::to_string((seconds / 60) % 60);
    string ss =std::to_string(seconds % 60);
    string time = hh.insert(0, 2 - hh.size(), '0') + ":" + 
                  mm.insert(0, 2 - mm.size(), '0') + ":" + 
                  ss.insert(0, 2 - ss.size(), '0');
    return time; 
}