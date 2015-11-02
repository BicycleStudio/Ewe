#ifndef GRAPHIC_FACADE_H_
#define GRAPHIC_FACADE_H_

namespace graphic {

  class IGraphicFacade {
  protected:
    virtual bool _initializeGraphic(int hwnd, int, int) = 0;
    virtual bool _resizeBuffers(int, int) = 0;
    virtual  void _beginScene() = 0;
    virtual void _endScene() = 0;
    virtual void _shutdown() = 0;
    
    bool _initialized;
    int _sizeX;
    int _sizeY;
  };

}

#endif //GRAPHIC_FACADE_H_
