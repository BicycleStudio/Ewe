#ifndef THREAD_MANAGER_H_
#define THREAD_MANAGER_H_

#include <vector>
#include <thread>
#include <queue>

#include <CommandManager.h>

#include "ThreadSubject.h"

namespace thread_manager {

  class ThreadManager {
  private:
    std::vector<ThreadSubject*> _subjects;
    std::vector<std::thread> _threads;

    command_manager::CommandManager _commandManager;
    std::shared_ptr<std::queue<command_manager::Command>> _commands;

    void _sendKillWindow();
  public:
    ThreadManager();

    void add(ThreadSubject *);
    void start();
    void stop();
    void pause();
    void resume();

    void listen();
  };

}

#endif // THREAD_MANAGER_H_
