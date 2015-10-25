#ifndef DIRECT_INPUT_FACADE_H_
#define DIRECT_INPUT_FACADE_H_

#include "DependenciesDInput.h"
#include "Dependencies.h"
#include "IOFacade.h"

namespace io {

  class DirectInputFacade : public IOFacade {
  public:
    DirectInputFacade();

  protected:
    bool _initialize(int hwnd);
    void _shutdown();
    bool _acquire();
    bool _update();
    bool _pressed(int key);

  private:
    IDirectInput8*        _device;
    IDirectInputDevice8*  _mouse;
    IDirectInputDevice8*  _keyboard;

    DIMOUSESTATE  _mouseState;
    bool          _keyState[256];
  };

}

#endif // DIRECT_INPUT_FACADE_H_
