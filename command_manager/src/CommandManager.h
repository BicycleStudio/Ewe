#ifndef COMMAND_MANAGER_H_
#define COMMAND_MANAGER_H_

#include <map>
#include <memory>
#include <queue>

namespace command_manager {
  enum ID {
    GRAPHIC,
    IO,
    LOGIC,
    SOUND,
    THREAD_MANAGER,
    WINDOW_FACADE
  };

  enum CommandType {
    // ONLY FOR WINDOW_FACADE!! if you wan't to shutdown system use KILL.
    DESTROY_ALL,
    // Any thread send KILL, ThreadManager send it to WindowFacade, WindowFacade after shutdown send DESTROY_ALL
    KILL,
    PAUSE,
    RESUME,
    INITIALIZE,
    RESIZE,
    INIT_BACKGROUND_AUDIO
  };

  class Command {
  public:
    Command(ID from_, ID to_, CommandType commandType_) :from(from_), to(to_), commandType(commandType_) { };
    ID from, to;
    CommandType commandType;
    std::vector<int> argInt;
    std::vector<std::string> argStr;
    std::vector<float> argFlt;
  };

  class CommandManager {
    std::map<ID, std::shared_ptr<std::queue<Command>>> queues_;
    std::queue<Command> commands_;
  public:
    void addQueue (ID, std::shared_ptr<std::queue<Command>>);
    void push (Command&);
    void process ( );
  };
}

#endif // COMMAND_MANAGER_H_