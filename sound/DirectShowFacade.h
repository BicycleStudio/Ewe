#ifndef DIRECT_SHOW_FACADE_H_
#define DIRECT_SHOW_FACADE_H_

#include "DependenciesDShow.h"
#include "SoundFacade.h"
#include "Logger.h"

namespace sound {

  class DirectShowFacade : public SoundFacade {
    utils::Logger* log;

  public:
    DirectShowFacade();
    ~DirectShowFacade();

  protected:
    bool _initialize(int hwnd);
    void _shutdown();

    void _pause();
    void _resume();
  };

}

#endif // DIRECT_SHOW_FACADE_H_