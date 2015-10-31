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
      struct WavFormat {
      private:
        WavFormat() {}
      public:
        WavFormat(int samplePerSecond, int bitsPerSample, int numChannels) {
          format.wFormatTag = WAVE_FORMAT_PCM;
          format.nSamplesPerSec = samplePerSecondPrimary;
          format.wBitsPerSample = bitzPerSamplePrimary;
          format.nChannels = channelsPrimary;
          format.nBlockAlign = (format.wBitsPerSample / 8) * format.nChannels;
          format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
          format.cbSize = 0;
        }
        WAVEFORMATEX format;
      };

      bool _setPrimaryBufferFormat(WAVEFORMATEX&);

      IDirectSound8*      _dSound;
      IDirectSoundBuffer* _primaryBuffer;
      std::vector<Audio>  _audios;
    };
  
  }

}

#endif // DIRECT_SOUND_FACADE_H_
