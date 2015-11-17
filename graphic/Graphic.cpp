#include "Graphic.h"

#include <iostream>
#include <CommandManager.h>

using std::cout;
using graphic::Graphic;

static const int graphicSleep = 100;

command_manager::ID graphic::Graphic::id() {
  return command_manager::ID::GRAPHIC;
}

Graphic::Graphic ( ) {
  log = new utils::Logger(typeid(*this).name());
  _initialized = false;
}

Graphic::~Graphic() {
  delete log;
}

void Graphic::stop() {
  log->info("thread was stopped");
  _shutdown();

  this->_willStop = true;
}

void Graphic::pause() {
  log->info("thread was paused");
  this->_paused = true;
}

void Graphic::resume() {
  log->info("thread was resumed");
  this->_paused = false;
}

void Graphic::_processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) { 
  case CommandType::INITIALIZE: 
    if(!_initializeGraphic(c.argInt[0], c.argInt[1], c.argInt[2])) {
      log->fatal("Can't initialize.");
      _sendKill();
    }
    _initialized = true;
    log->info("Initialize [OK]");
    break;
  case CommandType::RESIZE: 
    if (!_initialized) break;
    
    if(!_resizeBuffers(c.argInt[0], c.argInt[1])) {
      log->fatal("Can't resize buffers.");
      _sendKill();
    }
    break;
  default: break;
  }
  return;
}

void Graphic::start() {
  log->info("thread was started");

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(graphicSleep);
    std::this_thread::sleep_for (a);

    if (_initialized && !_paused) {
      _beginScene();
      _drawContent();
      _endScene();
    }

    _processCommands ( );
  }
}
