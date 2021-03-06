#ifndef GRAPHIC_DEPENDENCIES_H_
#define GRAPHIC_DEPENDENCIES_H_

#define __DX_GRAPHIC

#if defined(__DX_GRAPHIC)
#include "graphicFacade\directXFacade\DXFacade.h"
using namespace graphic::direct_x;
#elif defined(__GL_GRAPHIC)
#include "graphicFacade\openGLFacade\OpenGLFacade.h"
using namespace graphic::open_gl;
#endif

#pragma comment (lib, "patterns.lib")
#pragma comment (lib, "utils.lib")
#pragma comment (lib, "thread_manager.lib")
#pragma comment (lib, "data.lib")

#endif // GRAPHIC_DEPENDENCIES_H_
