#ifndef SINGLETON_H_
#define SINGLETON_H_

namespace patterns {

  template <class T>
  class Singleton {
  private:
    static T* p_instance;
  protected:
    T& operator=(T&);
  public:
    static T* getInstance() {
      if (!p_instance) {
        p_instance = new T();
      }
      return p_instance;
    }
  };

  template <class T>
  T* Singleton<T>::p_instance = 0;

}

#endif // SINGLETON_H_
