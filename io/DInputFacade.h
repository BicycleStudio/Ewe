#ifndef DIRECT_INPUT_FACADE_H_
#define DIRECT_INPUT_FACADE_H_

#include <Logger.h>

#include "DependenciesDInput.h"
#include "IInputFacade.h"

namespace io {

  namespace direct_input {

    class InputFacade : public IInputFacade {
      utils::Logger* log;

    public:
      InputFacade();
      ~InputFacade();

    protected:
      bool _initialize(int hwnd);
      void _shutdown();
      bool _acquire();
      void _update();
      bool _pressed(int key);

    private:
      IDirectInput8*        _device;
      IDirectInputDevice8*  _mouse;
      IDirectInputDevice8*  _keyboard;

      DIMOUSESTATE  _mouseState;
      bool          _keyState[256];
    };

  }

}

#endif // DIRECT_INPUT_FACADE_H_
