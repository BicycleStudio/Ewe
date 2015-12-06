#include "IO.h"
#include "IOKeys.h"

#include <iostream>

using std::cout;
using namespace io;
static const int ioSleep = 100;

command_manager::ID IO::id() {
  return command_manager::ID::IO;
}

IO::IO() { 
  _initialized = false;

  log = new utils::Logger(typeid(*this).name());
}

IO::~IO() {
  delete log;
}

void IO::start() {
  log->info("IO thread was started");

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(ioSleep);
    std::this_thread::sleep_for(a);

    _processCommands();
    
    if (_paused || !_initialized) 
      continue;

    _update();

    /*
    How To use system:
    if (_pressed(IO_KEY_W)) 
      cout << "W pressed!\n";
    if (_pressed(IO_KEY_A)) 
      cout << "A pressed!\n";
      */
  }
}

void IO::stop() {
  log->info("IO thread was stopped");
  _shutdown();

  this->_willStop = true;
}

void IO::pause() {
  log->info("IO thread was paused");
  _paused = true;
}

void IO::resume() {
  _paused = false;

  if (_initialized && !_acquire())
    _sendKill();

  log->info("IO thread was resumed");
}

void IO::_processCommand(command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::INITIALIZE: 
    log->info("IO initialize start...");
    if (!_initialize(c.argInt[0])) {
      log->fatal("_initialize error");
      _sendKill();
    }

    if (_initialized && !_acquire()) {
      log->fatal("_acquire error");
      _sendKill();
    }

    log->info("IO initialize [OK]");
    break;
  default: break;
  }
  return;
}