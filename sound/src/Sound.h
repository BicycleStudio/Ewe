#ifndef SOUND_H_
#define SOUND_H_

#include "Dependencies.h"
#include <ThreadManager.h>
#include <Logger.h>

namespace sound {

  class Sound : public thread_manager::ThreadSubjectWithKill, public SoundFacade
  {
    utils::Logger* log;

    void _processCommand(command_manager::Command& c) override final;
  public:
    command_manager::ID id() override final;
    Sound();
    ~Sound();

    void stop() override final;
    void start() override final;
    void pause() override final;
    void resume() override final;
  };

}

#endif // SOUND_H_
