#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#define DX

#ifdef DX
#include "DirectXFacade.h"
#elif GL
#include "OpenGLFacade.h"
#endif

#include <memory>
#include <queue>

#include <ThreadManager.h>

#include "GraphicSupport.h"

namespace graphic {

  class Graphic : public thread_manager::ThreadSubject, private GraphicSupport, 
#ifdef DX
    public DirectXFacade
#elif GL
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

    bool _resize(int sizeX, int sizeY);
  };

}

#endif // GRAPHIC_H_
