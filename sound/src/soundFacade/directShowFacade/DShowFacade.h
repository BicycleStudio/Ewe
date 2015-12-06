#ifndef DIRECT_SHOW_FACADE_H_
#define DIRECT_SHOW_FACADE_H_

#include "..\ISoundFacade.h"
#include "Logger.h"
#include "DShowAudio.h"

namespace sound {

  namespace direct_show {

    class SoundFacade : public ISoundFacade {
      utils::Logger* log;

    public:
      SoundFacade();
      ~SoundFacade();

    protected:
      bool _initialize(int hwnd) override final;
      void _shutdown() override final;

      void _pause() override final;
      void _resume() override final;

      void _initBackgroundAudio(const char*) override final;

    private:
      // audios except background
      std::vector<Audio> _audios;
      Audio _background;
    };
    
  }

}

#endif // DIRECT_SHOW_FACADE_H_