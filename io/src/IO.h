#ifndef IO_H_
#define IO_H_

#include <ThreadManager.h>
#include <GraphicInformer.h>
#include <Logger.h>
#include "Dependencies.h"

namespace io {

  class IO : public thread_manager::ThreadSubjectWithKill, public direct_input::InputFacade {
    utils::Logger* log;

    graphic::GraphicInformer graphicInformer;
    void _processCommand (command_manager::Command& c);
  public:
    command_manager::ID id();
    IO();
    ~IO();

    void processTick() override final;
    void stop() override final;
    void pause() override final;
    void resume() override final;
  };

}

#endif // IO_H_
