#include "ThreadSubject.h"

static const int threadManagerSleep = 100;

using std::shared_ptr;
using std::make_shared;
using std::queue;

using namespace thread_manager;
using command_manager::CommandManager;
using command_manager::Command;
using command_manager::CommandType;
using command_manager::ID;

ThreadSubject::ThreadSubject() {
  this->_commands = make_shared<queue<Command>>();
  this->_willStop = false;
  this->_paused = false;

  static shared_ptr<queue<Command>> _commandsAfterInit = make_shared<queue<Command>>();
  static bool _threadInit = false;

  _computeCommand = [this]() {
    while(this->_commands->size() > 0) {
      auto& c = this->_commands->front();
      if(c.commandType == CommandType::INITIALIZE) {
        _processCommand(c);
        _threadInit = true;
        while(_commandsAfterInit->size()) {
          auto& com = _commandsAfterInit->front();
          _processCommand(com);
          _commandsAfterInit->pop();
        }
        _computeCommand = [this]() {
          while(this->_commands->size() > 0) {
            auto& c = this->_commands->front();
            _processCommand(c);
            this->_commands->pop();
          }
        };
      }
      else {
        if(!_threadInit) _commandsAfterInit->push(c);
        else _processCommand(c);
      }
      this->_commands->pop();
    }
  };
}

void ThreadSubject::_processCommands() {
  _computeCommand();
}

void ThreadSubject::bind(CommandManager* commandManager) {
  _commandManager = commandManager;
}

shared_ptr<queue<Command>> ThreadSubject::getQueueLink() {
  return this->_commands;
}

void ThreadSubjectWithKill::_sendKill() {
  Command commandKill = Command(
    this->id(), ID::WINDOW_FACADE,
    CommandType::KILL);
  _commandManager->push(commandKill);
}
