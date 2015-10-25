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
}

void io::IO::start() {
  cout << "IO thread was started\n";

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(ioSleep);
    std::this_thread::sleep_for(a);

    processCommands();
    
    if (_paused || !_initialized) 
      continue;

    _update();

    if (_pressed(IO_KEY_W)) cout << "W pressed!\n";
    if (_pressed(IO_KEY_A)) cout << "A pressed!\n";
  }
}

void io::IO::stop() {
  cout << "IO thread was stopped\n";
  _shutdown();

  this->_willStop = true;
}

void io::IO::pause() {
  cout << "IO thread was paused\n";
  _paused = true;
}

void io::IO::resume() {
  _paused = false;

  if (_initialized && !_acquire())
    _sendKill();

  cout << "IO thread was resumed\n";
}

void io::IO::processCommand(command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::INITIALIZE: 
    cout << "IO initialize start...\n"; 
    if (!_initialize(c.args[0])) 
      _sendKill();

    if (_initialized && !_acquire())
      _sendKill();

    cout << "IO initialize [OK]\n";
    break;
  default: break;
  }
  return;
}