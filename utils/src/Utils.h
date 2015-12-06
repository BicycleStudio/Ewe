#ifndef UTILS_H_
#define UTILS_H_

#include <string>
//#include <wstring>

using std::string;
using std::wstring;

namespace utils {
  // TODO : Connects #100 explicit -> implicit!
  static std::wstring getWstring(string str) {
    return wstring(str.begin(), str.end());
  }

}


#endif