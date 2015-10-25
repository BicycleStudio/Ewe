#ifndef SOUND_FACADE_H_
#define SOUND_FACADE_H_

#include <Facade.h>

namespace sound {

  class SoundFacade : public patterns::Facade {
  protected:
    virtual bool _initialize(int hwnd) = 0;
    virtual void _shutdown() = 0;

    bool _initialized;
  };

}

#endif // SOUND_FACADE_H_
