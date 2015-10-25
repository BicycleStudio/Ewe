#ifndef IOFACADE_H_
#define IOFACADE_H_

#include <Facade.h>

namespace io  {

  class IOFacade : public patterns::Facade {
  protected:
    virtual bool _initialize(int hwnd) = 0;
    virtual void _shutdown() = 0;
    virtual bool _acquire() = 0;
    virtual void _update() = 0;

    bool _initialized;

    virtual bool _pressed(int key) = 0;
  };

}

#endif // IOFACADE_H_
