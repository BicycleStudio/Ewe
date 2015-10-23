#include "Dependencies.h"

#include <ThreadManager.h>

#include <WindowFacade.h>
#include <Graphic.h>
#include <Logic.h>
#include <IO.h>
#include <Sound.h>

int main ( ) {
  thread_manager::ThreadManager tm;
  
  window_facade::WindowFacade* window = window_facade::WindowFacade::getInstance();
  graphic::Graphic graphic;
  logic::Logic logic;
  io::IO io;
  sound::Sound sound;
  
  tm.add(&graphic);
  tm.add(&io);
  tm.add(&logic);
  tm.add(&sound);
  tm.add(window);

  tm.start();

  tm.listen ( );

  return 0;
}