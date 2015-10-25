#include "Graphic.h"

#include <iostream>

#include <CommandManager.h>

using std::cout;
static const int graphicSleep = 100;

command_manager::ID graphic::Graphic::id() {
  return command_manager::ID::GRAPHIC;
}

graphic::Graphic::Graphic ( ) {
  _initialized = false;
}

void graphic::Graphic::stop() {
  cout << "Graphic thread was stopped\n";
  _shutdown();

  this->_willStop = true;
}
void graphic::Graphic::pause() {
  this->_paused = true;
}
void graphic::Graphic::resume() {
  this->_paused = false;
}
void graphic::Graphic::processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) { 
  case CommandType::INITIALIZE: 
    cout << "Graphic init\thwnd = " << c.args[0] << "\tsizeX = " << c.args[1] << "\tsizeY = " << c.args[2] <<"\n";
    if (!_initializeGraphic(c.args[0], c.args[1], c.args[2])) _sendKill();
    _initialized = true;
    cout << "Graphic init [OK]\n";
    break;
  case CommandType::PAUSE: cout << "Graphic pause"; break;
  case CommandType::RESUME: cout << "Graphic resume"; break;
  case CommandType::RESIZE: 
    if (!_initialized) break;
    
    cout << "Graphic resize\tx = " << c.args[0] << " y = " << c.args[1] << "\n";
    if (!_resizeBuffers(c.args[0], c.args[1]))
      _sendKill();
    break;
  default: break;
  }
  return;
}

void graphic::Graphic::start() {
  cout << "Graphic thread was started\n";

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(graphicSleep);
    std::this_thread::sleep_for (a);

    if (_initialized && !_paused) {
      _beginScene();
      // TODO: Render objects
      _endScene();
    }

    processCommands ( );
  }
}
