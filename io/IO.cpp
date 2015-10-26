#include "IO.h"
#include "IOKeys.h"

#include <iostream>

using std::cout;
static const int ioSleep = 100;

command_manager::ID io::IO::id() {
  return command_manager::ID::IO;
}

io::IO::IO() { 
  _initialized = false;

  log = new utils::Logger(typeid(*this).name());
}

io::IO::~IO() {
  delete log;
}

void io::IO::start() {
  log->info("IO thread was started");

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(ioSleep);
    std::this_thread::sleep_for(a);

    processCommands();
    
    if (_paused || !_initialized) 
      continue;

    _update();

    if (_pressed(IO_KEY_W)) 
      cout << "W pressed!\n";
    if (_pressed(IO_KEY_A)) 
      cout << "A pressed!\n";
  }
}

void io::IO::stop() {
  log->info("IO thread was stopped");
  _shutdown();

  this->_willStop = true;
}

void io::IO::pause() {
  log->info("IO thread was paused");
  _paused = true;
}

void io::IO::resume() {
  _paused = false;

  if (_initialized && !_acquire())
    _sendKill();

  log->info("IO thread was resumed");
}

void io::IO::processCommand(command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::INITIALIZE: 
    log->info("IO initialize start...");
    if (!_initialize(c.args[0])) {
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