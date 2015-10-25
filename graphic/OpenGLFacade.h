#ifndef OPENGL_FACADE_H_
#define OPENGL_FACADE_H_

#include "DependenciesGL.h"
#include "GraphicFacade.h"

namespace graphic {

  class OpenGLFacade : public GraphicFacade {
  public:
    OpenGLFacade();

  protected:
    bool _initializeGraphic(int hwnd, int, int);
    bool _resizeBuffers(int, int);
    void _beginScene();
    void _endScene();
    void _shutdown();

  private:
    HDC     _hDC;
  };

}

#endif // OPENGL_FACADE_H_