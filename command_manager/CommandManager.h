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
    WINDOW_FACADE,
    ALL
  };

  enum CommandType {
    KILL,
    PAUSE,
    ACTION,
    PLAY
  };

  class Command {
  public:
    Command(ID from_, ID to_) :from(from_), to(to_) {};
    Command(ID from_, ID to_, CommandType commandType_) :from(from_), to(to_), commandType(commandType_) {};
    ID from, to;
    CommandType commandType;
    int args [3];
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