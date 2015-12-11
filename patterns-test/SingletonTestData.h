#include <creational\Singleton.h>

class SingletonImpl : public patterns::Singleton<SingletonImpl>{
private:
  SingletonImpl() {}
  SingletonImpl(const SingletonImpl&) {}
  friend class patterns::Singleton<SingletonImpl>;

private:
  int _value;
public:
  void set(int v) {
    _value = v;
  }
  int get() {
    return _value;
  }
};

class SingletonImpl2 : public patterns::Singleton<SingletonImpl2>{
private:
  SingletonImpl2() {}
  SingletonImpl2(const SingletonImpl2&) {}
  friend class patterns::Singleton<SingletonImpl2>;

private:
  int _value;
public:
  void set(int v) {
    _value = v;
  }
  int get() {
    return _value;
  }
};

const int intTestValue = 42;
const int intTestClearValue = 0;
