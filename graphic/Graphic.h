#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include "DirectXFacade.h"

#include <memory>
#include <queue>

#include <ThreadManager.h>

#include "GraphicSupport.h"

namespace graphic {

<<<<<<< HEAD
class Graphic : public thread_manager::ThreadSubject, private GraphicSupport {
  void processCommand (command_manager::Command& c);
public:
  command_manager::ID id();
  void stop();
  void start();
  void pause();
  void resume();

  Graphic ( );
private:
  bool _initialize(HWND renderHwnd, int, int);
  void _shutdown();
  void _sendKill();

  bool _createDeviceSwapChain(HWND renderHwnd);
  bool _createRTV();
  bool _createDSV();
  bool _resize(int sizeX, int sizeY);
  bool _resizeRecreate(int sizeX, int sizeY);
  void _clearContext();
  void _setRenderTargets();

  void _beginScene();
  void _endScene();

private:
  ID3D11Device* _device;
  ID3D11DeviceContext* _immediateContext;
  IDXGISwapChain* _swapChain;
  ID3D11RenderTargetView* _renderTargetView;
  ID3D11Texture2D* _backBuffer;
  ID3D11DepthStencilView*_depthStencilView;
  ID3D11Texture2D* _depthStencil;

  int _sizeX;
  int _sizeY;
  bool _initialized;
  float* _sceneColor;
};
=======
  class Graphic : public thread_manager::ThreadSubject, private GraphicSupport, public DirectXFacade {
    void processCommand (command_manager::Command& c);
  public:
    command_manager::ID id();
    void stop();
    void start();

    Graphic ( );
  private:
    bool _initialize(HWND renderHwnd, int, int);
    void _sendKill();

    bool _resize(int sizeX, int sizeY);
  };
>>>>>>> DirectXFacade, Graphic implements this facade

}

#endif // GRAPHIC_H_
