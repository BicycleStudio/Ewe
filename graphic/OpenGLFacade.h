#ifndef OPENGL_FACADE_H_
#define OPENGL_FACADE_H_

#include "DependenciesGL.h"
#include "IGraphicFacade.h"

namespace graphic {

  namespace open_gl {

    class GraphicFacade : public IGraphicFacade {
    public:
      GraphicFacade();
      ~GraphicFacade();

    protected:
      bool _initializeGraphic(int hdc, int, int);
      bool _resizeBuffers(int, int);
      void _beginScene();
      void _endScene();
      void _shutdown();

    private:
      HDC     _hDC;
      HGLRC   _hRC;
    };

  }
}

#endif // OPENGL_FACADE_H_