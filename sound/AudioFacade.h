#ifndef AUDIO_H
#define AUDIO_H
#include <string>

namespace sound {

  class AudioFacade {
  public:
    virtual bool initialize(std::string fileName, int forDSound) = 0;
    virtual void shutdown() = 0;
    virtual bool pause() = 0;
    virtual bool run() = 0;
    virtual bool stop() = 0;
  };

}

#endif AUDIO_H
