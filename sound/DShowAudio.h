#ifndef DSHOW_AUDIO_H_
#define DSHOW_AUDIO_H_

#include "DependenciesDShow.h"
#include <string>

namespace sound {

  namespace direct_show {

    class Audio {
    public:
      Audio();
      ~Audio();

      HRESULT initialize(std::string fileName);
      void shutdown();
      HRESULT pause();
      HRESULT run();
      HRESULT stop();

    private:
      IGraphBuilder* _graphBuilder;
      IMediaControl* _mediaControl;
      IMediaEvent*   _mediaEvent;
    };

  }

}

#endif //DSHOW_AUDIO_H_