#ifndef GRAPHIC_FACADE_H_
#define GRAPHIC_FACADE_H_

namespace graphic {

  class IGraphicFacade {
  protected:
    virtual bool _initializeGraphic(int hwnd, int, int) = 0;
    virtual bool _resizeBuffers(int, int) = 0;
    virtual void _beginScene() = 0;
    virtual void _drawContent() = 0;
    virtual void _endScene() = 0;
    virtual void _shutdown() = 0;
    virtual bool _addModel(const char*) = 0;

    int _sizeX;
    int _sizeY;

    bool _initialized;
  };

}

#endif //GRAPHIC_FACADE_H_
