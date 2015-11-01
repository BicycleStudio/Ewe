#ifndef SOUND_FACADE_H_
#define SOUND_FACADE_H_

namespace sound {

  class ISoundFacade {
  protected:
    virtual bool _initialize(int hwnd) = 0;
    virtual void _shutdown() = 0;

    virtual void _pause() = 0;
    virtual void _resume() = 0;
    bool _initialized;
  };

}

#endif // SOUND_FACADE_H_
