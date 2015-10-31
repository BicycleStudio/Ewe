#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <memory>
#include <queue>
#include <ThreadManager.h>
#include "GraphicSupport.h"

namespace graphic {

  class Graphic : public thread_manager::ThreadSubjectWithKill, public GraphicFacade, private GraphicSupport {
    void _processCommand (command_manager::Command& c);
  public:
    command_manager::ID id();
    void stop();
    void start();
    void pause();
    void resume();

    Graphic ( );
  };

}

#endif // GRAPHIC_H_
