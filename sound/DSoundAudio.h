#ifndef DSOUND_AUDIO_H_
#define DSOUND_AUDIO_H_

#include "DependenciesDSound.h"
#include <string>

namespace sound {

  namespace direct_sound {

    class Audio  {
    public:
      Audio();

      bool initialize(std::string fileName, IDirectSound8* dsound);
      void shutdown();
      bool pause();
      bool run();
      bool stop();

    private:
      typedef struct WaveHeader {
        char  riffSig[4];        // 'RIFF'
        long  waveformChunkSize;
        char  waveSig[4];        // 'WAVE'
        char  formatSig[4];      // 'fmt ' 
        long  formatChunkSize;
        short formatTag;         // WAVE_FORMAT_PCM
        short channels;
        long  sampleRate;
        long  bytesPerSec;
        short blockAlign;
        short bitsPerSample;
        char  dataSig[4];        // 'data'
        long  dataSize;
      } WaveHeader;

      bool _update(std::string fileName, long LockPos, long Size);

      bool _create(IDirectSound8* dSound, long dataSize, WAVEFORMATEX&);
      bool _create(IDirectSound8* dSound, long dataSize, WAVEFORMATEX&, DWORD flags);
      bool _create(IDirectSound8* dSound, DSBUFFERDESC&);

      IDirectSoundBuffer8* _buffer;
    };

  }

}

#endif //DSOUND_AUDIO_H_