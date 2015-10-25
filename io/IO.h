#ifndef IO_H_
#define IO_H_
#define __DIRECT_INPUT

#if defined(__DIRECT_INPUT)
#include "DirectInputFacade.h"
#endif

#include <ThreadManager.h>
#include <GraphicInformer.h>

namespace io {

  class IO : public thread_manager::ThreadSubject,
  #if defined(__DIRECT_INPUT)
    public DirectInputFacade
  #endif
  {
    graphic::GraphicInformer graphicInformer;
    void processCommand (command_manager::Command& c);
  public:
    command_manager::ID id();
    IO();

    void stop();
    void start();
    void pause();
    void resume();

  private:
    void _sendKill();
    bool _paused;
  };

}

#endif // IO_H_
