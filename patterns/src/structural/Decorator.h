#ifndef DECORATOR_H_
#define DECORATOR_H_

namespace patterns {

  template<class T>
  class Decorator : public T { 
  private:
    T* base_;
  public:
    Decorator (T* t) : base_ (t);
  };

}

#endif // DECORATOR_H_
