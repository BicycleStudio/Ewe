#ifndef DSHOW_AUDIO_H_
#define DSHOW_AUDIO_H_

#include "DependenciesDShow.h"
#include "AudioFacade.h"

namespace sound {

  namespace direct_show {

    class Audio : public AudioFacade {
    public:
      Audio();

      bool initialize(std::string fileName, int forDsound);
      void shutdown();
      bool pause();
      bool run();
      bool stop();

    private:
      IGraphBuilder* _graphBuilder;
      IMediaControl* _mediaControl;
      IMediaEvent*   _mediaEvent;
    };

  }

}

#endif //DSHOW_AUDIO_H_