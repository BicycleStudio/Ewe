#ifndef GRAPHIC_H_
#define GRAPHIC_H_

// Safe disable dx default warnings
#pragma warning(push)
#pragma warning(disable:4005)

#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <xnamath.h>

#include <memory>
#include <queue>

#include <ThreadManager.h>

#include "GraphicSupport.h"

namespace graphic {

class Graphic : public thread_manager::ThreadSubject, private GraphicSupport {
  void processCommand (command_manager::Command& c);
public:
  command_manager::ID id();
  void stop();
  void start();

  Graphic ( );
};

}

#pragma warning(pop)
#endif // GRAPHIC_H_
