#ifndef THREAD_MANAGER_H_
#define THREAD_MANAGER_H_

#include <vector>
#include <thread>
#include <queue>

#include <CommandManager.h>
#include "ThreadSubject.h"
using command_manager::CommandManager;
using command_manager::Command;

namespace thread_manager {

  class ThreadManager {
  private:
    std::vector<ThreadSubject*> _subjects;
    std::vector<std::thread> _threads;

    CommandManager _commandManager;
    std::shared_ptr<std::queue<Command>> _commands;

    void _sendKillWindow();
    CommandManager* getCommandManager();
  public:
    ThreadManager();

    bool processTick();
    void add(ThreadSubject *);
    void start();
    bool startMono();
    void stop();
    void pause();
    void resume();

    void listen();
    void listenMono();
  };

}

#endif // THREAD_MANAGER_H_
