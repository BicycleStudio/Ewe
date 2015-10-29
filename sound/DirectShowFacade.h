#ifndef DIRECT_SHOW_FACADE_H_
#define DIRECT_SHOW_FACADE_H_

#include "DependenciesDShow.h"
#include "SoundFacade.h"
#include "Logger.h"

namespace sound {

  namespace direct_show {

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
      IGraphBuilder* _graphBuilder;
      IMediaControl* _mediaControl;
      IMediaEvent*   _mediaEvent;

      IGraphBuilder* _graphBuilder2;
      IMediaControl* _mediaControl2;
      IMediaEvent*   _mediaEvent2;
    };
    
  }

}

#endif // DIRECT_SHOW_FACADE_H_