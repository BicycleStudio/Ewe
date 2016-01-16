#ifndef LOGIC_H_
#define LOGIC_H_

#include "Dependencies.h"

#include <ThreadManager.h>
#include <Logger.h>

namespace logic {

  class Logic : public thread_manager::ThreadSubject {
    utils::Logger* log;

    void _processCommand (command_manager::Command& c);
  public:
    command_manager::ID id();
    Logic();
    ~Logic();

    void processTick() override final;
    void stop() override final;
    void pause() override final;
    void resume() override final;
  };

}

#endif // LOGIC_H_
