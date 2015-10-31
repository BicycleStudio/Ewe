#ifndef SOUND_H_
#define SOUND_H_

#include "Dependencies.h"
#include <ThreadManager.h>
#include <Logger.h>

namespace sound {

  class Sound : public thread_manager::ThreadSubjectWithKill, public SoundFacade
  {
    utils::Logger* log;

    void _processCommand(command_manager::Command& c);
  public:
    command_manager::ID id();
    Sound();
    ~Sound();

    void stop();
    void start();
    void pause();
    void resume();
  };

}

#endif // SOUND_H_
