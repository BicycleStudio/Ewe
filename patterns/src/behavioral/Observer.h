#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <vector>
#include <memory>

namespace patterns {

  using std::vector;
  using std::shared_ptr;

  template <class Entity, class Event>
  class Observer {
  public:
    virtual ~Observer () { }
    virtual void onNotify (const Entity& entity, Event event) = 0;
  };

  template <class Entity, class Event>
  class ObserverSubject {
  private:
    vector<shared_ptr<Observer>> observers_;
  };

}

#endif // OBSERVER_H_
