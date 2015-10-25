#ifndef LOGIC_H_
#define LOGIC_H_

#include "Dependencies.h"

#include <ThreadManager.h>
#include <Logger.h>

namespace logic {

class Logic : public thread_manager::ThreadSubject {
  utils::Logger* log;

  void processCommand (command_manager::Command& c);
public:
  command_manager::ID id();
  Logic();
  ~Logic();

  void stop();
  void start();
  void pause();
  void resume();
};

}

#endif // LOGIC_H_
