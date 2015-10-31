#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <ThreadManager.h>
#include "GraphicSupport.h"
#include <Logger.h>

namespace graphic {

  class Graphic : public thread_manager::ThreadSubjectWithKill, public GraphicFacade, private GraphicSupport {
    utils::Logger* log;

    void _processCommand(command_manager::Command& c);
  public:
    command_manager::ID id();
    Graphic();
    ~Graphic();

    void stop();
    void start();
    void pause();
    void resume();
  };

}

#endif // GRAPHIC_H_
