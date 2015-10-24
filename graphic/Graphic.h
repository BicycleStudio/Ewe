#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#define __DX_GRAPHIC

#if defined(__DX_GRAPHIC)
#include "DirectXFacade.h"
#elif defined(__GL_GRAPHIC)
#include "OpenGLFacade.h"
#endif

#include <memory>
#include <queue>

#include <ThreadManager.h>

#include "GraphicSupport.h"

namespace graphic {

  class Graphic : public thread_manager::ThreadSubject, private GraphicSupport, 
#if defined(__DX_GRAPHIC)
    public DirectXFacade
#elif defined(__GL_GRAPHIC)
    public OpenGLFacade
#endif
  {
    void processCommand (command_manager::Command& c);
  public:
    command_manager::ID id();
    void stop();
    void start();

    Graphic ( );
  private:
    void _sendKill();
  };

}

#endif // GRAPHIC_H_
