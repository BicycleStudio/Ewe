#ifndef DSOUND_AUDIO_H_
#define DSOUND_AUDIO_H_

#include "DependenciesDSound.h"
#include "AudioFacade.h"

namespace sound {

  namespace direct_sound {

    class Audio : public AudioFacade {
    public:
      Audio();

      bool initialize(std::string fileName, int forDSound);
      void shutdown();
      bool pause();
      bool run();
      bool stop();

    private:
      bool _updateRandom();

      bool _create(IDirectSound8* dSound, int seconds, WAVEFORMATEX&);
      bool _create(IDirectSound8* dSound, int seconds, WAVEFORMATEX&, DWORD flags);
      bool _create(IDirectSound8* dSound, DSBUFFERDESC&);

      IDirectSoundBuffer8* _buffer;
    };

  }

}

#endif //DSOUND_AUDIO_H_