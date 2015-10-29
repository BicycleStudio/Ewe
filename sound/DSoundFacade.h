#ifndef DIRECT_SOUND_FACADE_H_
#define DIRECT_SOUND_FACADE_H_

#include "DependenciesDSound.h"
#include "SoundFacade.h"
#include "Logger.h"

namespace sound {

  namespace direct_sound {

    class Facade : public SoundFacade {
      utils::Logger* log;

    public:
      Facade();
      ~Facade();

    protected:
      bool _initialize(int hwnd);
      void _shutdown();

      void _pause();
      void _resume();
    private:
      bool _setPrimaryBufferFormat(WAVEFORMATEX&);
      bool _updateBuffer(IDirectSoundBuffer8*);

      IDirectSoundBuffer8* _createSecondaryBuffer(int seconds, WAVEFORMATEX&);
      IDirectSoundBuffer8* _createSecondaryBuffer(int seconds, WAVEFORMATEX&, DWORD flags);
      IDirectSoundBuffer8* _createSecondaryBuffer(DSBUFFERDESC&);

      IDirectSound8*      _dSound;
      IDirectSoundBuffer* _primaryBuffer;
      std::vector<IDirectSoundBuffer8*> _secondaryBuffers;
    };
  
  }

}

#endif // DIRECT_SOUND_FACADE_H_
