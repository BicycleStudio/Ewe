#ifndef SOUND_H_
#define SOUND_H_
#define __DX_SOUND

#include "Dependencies.h"

#ifdef __DX_SOUND
#include "DirectSoundFacade.h"
#endif

#include <ThreadManager.h>
#include <Logger.h>

namespace sound {

  class Sound : public thread_manager::ThreadSubjectWithKill,
  #ifdef __DX_SOUND
    public DirectSoundFacade
  #endif
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
