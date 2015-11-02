#ifndef IO_DEPENDENCIES_H_
#define IO_DEPENDENCIES_H_

#define __DIRECT_INPUT

#if defined(__DIRECT_INPUT)
#include "DInputFacade.h"
using io::direct_input::InputFacade;
#endif


#pragma comment (lib, "patterns.lib")
#pragma comment (lib, "utils.lib")
#pragma comment (lib, "thread_manager.lib")
#pragma comment (lib, "data.lib")

#endif // IO_DEPENDENCIES_H_
