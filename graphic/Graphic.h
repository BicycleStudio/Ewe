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
private:
  bool _initialize(HWND renderHwnd, int, int);
  void _shutdown();
  void _sendKill();

  bool _createDeviceSwapChain(HWND renderHwnd);
  bool _createRTV();
  bool _resize(int sizeX, int sizeY);
  bool _resizeRecreate(int sizeX, int sizeY);
  void _clearContext();

private:
  ID3D11Device*           _device;
  ID3D11DeviceContext*    _immediateContext;
  IDXGISwapChain*         _swapChain;
  ID3D11RenderTargetView* _renderTargetView;
  ID3D11Texture2D*				_backBuffer;
  ID3D11DepthStencilView* _depthStencilView;
  ID3D11Texture2D*        _depthStencil;

  int	_sizeX;
  int	_sizeY;
  bool _initialized;
};

}

#pragma warning(pop)
#endif // GRAPHIC_H_
