#ifndef DIRECT_SOUND_FACADE_H_
#define DIRECT_SOUND_FACADE_H_

#include "DependenciesDSound.h"
#include "ISoundFacade.h"
#include "DSoundAudio.h"
#include "Logger.h"

namespace sound {

  namespace direct_sound {

    class SoundFacade : public ISoundFacade {
      utils::Logger* log;

    public:
      SoundFacade();
      ~SoundFacade();

    protected:
      bool _initialize(int hwnd);
      void _shutdown();

      void _pause();
      void _resume();
    private:
      bool _setPrimaryBufferFormat(WAVEFORMATEX&);


      IDirectSound8*      _dSound;
      IDirectSoundBuffer* _primaryBuffer;
      std::vector<Audio>  _audios;
    };
  
  }

}

#endif // DIRECT_SOUND_FACADE_H_
