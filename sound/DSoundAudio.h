#ifndef DSOUND_AUDIO_H_
#define DSOUND_AUDIO_H_

#include "DependenciesDSound.h"
#include <string>
#include <Logger.h>

namespace sound {

  namespace direct_sound {

    class Audio  {
      utils::Logger* log;

    public:
      Audio();
      ~Audio();

      bool initialize(std::string fileName, IDirectSound8* dsound);
      void shutdown();
      bool pause();
      bool run();
      bool stop();

    private:
      bool _update(std::string fileName, long LockPos, long Size);

      bool _create(IDirectSound8* dSound, long dataSize, WAVEFORMATEX&, DWORD flags);
      bool _create(IDirectSound8* dSound, long dataSize, WAVEFORMATEX&);
      bool _create(IDirectSound8* dSound, DSBUFFERDESC&);

      IDirectSoundBuffer8* _buffer;
    };

  }

}

#endif //DSOUND_AUDIO_H_