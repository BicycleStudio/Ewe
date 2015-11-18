#ifndef SOUND_DEPENDENCIES_H_
#define SOUND_DEPENDENCIES_H_

//#define __DX_SOUND
#define __DX_SHOW

#if defined(__DX_SOUND)
#include "DSoundFacade.h"
using sound::direct_sound::SoundFacade;
#elif defined(__DX_SHOW)
#include "DShowFacade.h"
using sound::direct_show::SoundFacade;
#endif

#pragma comment (lib, "patterns.lib")
#pragma comment (lib, "utils.lib")
#pragma comment (lib, "thread_manager.lib")
#pragma comment (lib, "data.lib")

#endif // SOUND_DEPENDENCIES_H_
