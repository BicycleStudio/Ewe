#ifndef DIRECT_SHOW_FACADE_H_
#define DIRECT_SHOW_FACADE_H_

#include "SoundFacade.h"
#include "Logger.h"
#include "DShowAudio.h"

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
      std::vector<Audio> _audios;
    };
    
  }

}

#endif // DIRECT_SHOW_FACADE_H_