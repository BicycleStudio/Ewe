#ifndef WAV_FORMAT_H_
#define WAV_FORMAT_H_

#include "DependenciesDSound.h"

namespace sound {
  namespace direct_sound {

    class WavFormat {
    private:
      WavFormat() {}
    public:
      WavFormat(int samplePerSecond, int bitsPerSample, int numChannels) {
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.nSamplesPerSec = samplePerSecond;
        format.wBitsPerSample = bitsPerSample;
        format.nChannels = numChannels;
        format.nBlockAlign = (format.wBitsPerSample / 8) * format.nChannels;
        format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
        format.cbSize = 0;
      }
      WAVEFORMATEX format;
    };

    struct WavHeader {
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
    };

  }
}

#endif // WAV_FORMAT_H_