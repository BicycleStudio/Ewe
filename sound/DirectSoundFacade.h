#ifndef DIRECT_SOUND_FACADE_H_
#define DIRECT_SOUND_FACADE_H_

#include "DependenciesDSound.h"
#include "SoundFacade.h"
#include "Logger.h"

namespace sound {

  class DirectSoundFacade : public SoundFacade {
    utils::Logger* log;

  public:
    DirectSoundFacade();
    ~DirectSoundFacade();

  protected:
    bool _initialize(int hwnd);
    void _shutdown();

  private:
    bool _setPrimaryBufferFormat(WAVEFORMATEX&);
    bool _updateBuffer(IDirectSoundBuffer8*);

    IDirectSoundBuffer8* _createSecondaryBuffer(int seconds, WAVEFORMATEX&);
    IDirectSoundBuffer8* _createSecondaryBuffer(int seconds, WAVEFORMATEX&, DWORD flags);
    IDirectSoundBuffer8* _createSecondaryBuffer(DSBUFFERDESC&);

    IDirectSound8*      _dSound;
    IDirectSoundBuffer* _primaryBuffer;
    std::vector<IDirectSoundBuffer8*> _secondaryBuffers;
  };

}

#endif // DIRECT_SOUND_FACADE_H_
