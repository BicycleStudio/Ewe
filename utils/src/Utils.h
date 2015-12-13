#ifndef UTILS_H_
#define UTILS_H_

#include <string>

namespace utils {
  static void operator%= (std::wstring& dst, const std::string& src) {
    dst = std::wstring(src.begin(), src.end());
  }

  class Method {
  private:
    void(*function)(void*);
    void *parameters[4];

    template<class T>
    void _set(T func) {
      sizeof(int[sizeof(parameters) -sizeof(T)]);
      function = [](void *param) { (*(T*) param)(); };
      memcpy(parameters, &func, sizeof(T));
    }

  public:
    template<class T>
    Method(T func) {
      _set(func);
    }
    Method() {
      function = [](void*) {};
    }

    template<class T>
    void operator=(T func) {
      _set(func);
    }
    void operator()() { function(parameters); }
  };

}


#endif