#include <queue>

#include <CommandManager.h>
#include <Utils.h>

namespace thread_manager {

  class ThreadSubject {
  protected:
    using threadSubjectMethod = void(*)(ThreadSubject*);
    threadSubjectMethod _computeCommand;

    command_manager::CommandManager* _commandManager;

    std::shared_ptr<std::queue<command_manager::Command>> _commands;
    void _processCommands();
    virtual void _processCommand(command_manager::Command& c) = 0;

    bool _willStop;
    bool _paused;
    int _sleepThread;
  public:
    ThreadSubject();
    virtual bool initialize();
    virtual void processTick() = 0;

    std::shared_ptr<std::queue<command_manager::Command>> getQueueLink();
    virtual command_manager::ID id() = 0;

    virtual void stop() = 0;
    void start();

    void bind(command_manager::CommandManager*);

    virtual void pause() = 0;
    virtual void resume() = 0;
  };

  class ThreadSubjectWithKill : public ThreadSubject {
  protected:
    void _sendKill();
  };

}