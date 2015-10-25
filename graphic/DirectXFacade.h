#ifndef DIRECTX_FACADE_H_
#define DIRECTX_FACADE_H_

#include "DependenciesDX.h"
#include "GraphicFacade.h"

namespace graphic {

  class DirectXFacade : public GraphicFacade  {
  public:
    DirectXFacade();

  protected:
    bool _initializeGraphic(int hwnd, int, int);
    bool _resizeBuffers(int, int);
    void _beginScene();
    void _endScene();
    void _shutdown();
    
  private:
    bool _createDeviceSwapChain(HWND renderHwnd);
    bool _createRTV();
    bool _createDSV();
    void _clearContext();
    void _setRenderTargets();

    ID3D11Device*           _device;
    ID3D11DeviceContext*    _immediateContext;
    IDXGISwapChain*         _swapChain;
    ID3D11RenderTargetView* _renderTargetView;
    ID3D11Texture2D*        _backBuffer;
    ID3D11DepthStencilView* _depthStencilView;
    ID3D11Texture2D*        _depthStencil;
  };

}
#endif // DIRECTX_FACADE_H_