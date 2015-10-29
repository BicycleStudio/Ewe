#ifndef SOUND_H_
#define SOUND_H_
#define __DX_SOUND
//#define __DX_SHOW

#include "Dependencies.h"

#if defined(__DX_SOUND)
#include "DSoundFacade.h"
#elif defined(__DX_SHOW)
#include "DShowFacade.h"
#endif

#include <ThreadManager.h>
#include <Logger.h>

namespace sound {

  class Sound : public thread_manager::ThreadSubjectWithKill,
  #ifdef __DX_SOUND
    public direct_sound::Facade
  #elif defined(__DX_SHOW)
    public direct_show::Facade
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
