#ifndef IINPUT_FACADE_H
#define IINPUT_FACADE_H

namespace io  {

  class IInputFacade  {
  protected:
    virtual bool _initialize(int hwnd) = 0;
    virtual void _shutdown() = 0;
    virtual bool _acquire() = 0;
    virtual void _update() = 0;

    bool _initialized;

    virtual bool _pressed(int key) = 0;
  };

}

#endif // IINPUT_FACADE_H
