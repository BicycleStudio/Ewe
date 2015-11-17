#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <ThreadManager.h>
#include "GraphicSupport.h"
#include <Logger.h>

namespace graphic {

  class Graphic : public thread_manager::ThreadSubjectWithKill, public GraphicFacade, private GraphicSupport {
    utils::Logger* log;

    void _processCommand(command_manager::Command& c) override final;
  public:
    command_manager::ID id() override final;
    Graphic();
    ~Graphic();

    void stop() override final;
    void start() override final;
    void pause() override final;
    void resume() override final;
  };

}

#endif // GRAPHIC_H_
