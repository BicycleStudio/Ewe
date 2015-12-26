#ifndef UTILS_H_
#define UTILS_H_

#include <string>

namespace utils {
  static void operator%= (std::wstring& dst, const std::string& src) {
    dst = std::wstring(src.begin(), src.end());
  }
}


#endif