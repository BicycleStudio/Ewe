#ifndef DIRECT_SOUND_FACADE_H_
#define DIRECT_SOUND_FACADE_H_

#include "DependenciesDSound.h"
#include "SoundFacade.h"

namespace sound {

  class DirectSoundFacade : public SoundFacade {
  public:
    DirectSoundFacade();

  protected:
    bool _initialize(int hwnd);
    void _shutdown();
  };

}

#endif // DIRECT_SOUND_FACADE_H_
