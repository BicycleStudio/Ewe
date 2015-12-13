#ifndef UTILS_H_
#define UTILS_H_

#include <string>

namespace utils {
  static std::wstring& operator %= (std::wstring& dst, const std::string& src) {
    return std::wstring(src.begin(), src.end());
  }
}


#endif