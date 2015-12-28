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
  static auto standartProcessor = [](ThreadSubject* this_) {
    while(this_->_commands->size() > 0) {
      auto& c = this_->_commands->front();
      this_->_processCommand(c);
      this_->_commands->pop();
    }
  };
  static auto preInitProcessor = [](ThreadSubject* this_) {
    while(this_->_commands->size() > 0) {
      auto& c = this_->_commands->front();
      if(c.commandType == CommandType::INITIALIZE) {
        this_->_processCommand(c);
        _threadInit = true;
        while(_commandsAfterInit->size()) {
          auto& com = _commandsAfterInit->front();
          this_->_processCommand(com);
          _commandsAfterInit->pop();
        }
        this_->_computeCommand = standartProcessor;
      }
      else {
        if(!_threadInit) _commandsAfterInit->push(c);
        else this_->_processCommand(c);
      }
      this_->_commands->pop();
    }
  };
  _computeCommand = preInitProcessor;
}

void ThreadSubject::_processCommands() {
  _computeCommand(this);
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
