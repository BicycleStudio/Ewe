#ifndef DIRECTX_FACADE_H_
#define DIRECTX_FACADE_H_

#include "DependenciesDX.h"
#include "..\IGraphicFacade.h"
#include <Logger.h>
#include "DXModel.h"
#include "DXMaterial.h"

using std::vector;

namespace graphic {

  namespace direct_x {

    class GraphicFacade : public IGraphicFacade {
      utils::Logger* log;

    public:
      GraphicFacade();
      ~GraphicFacade();

    protected:
      bool _initializeGraphic(int hwnd, int, int);
      bool _resizeBuffers(int, int);
      void _beginScene();
      void _drawContent();
      void _endScene();
      void _shutdown();
      bool _addModel(const char*);

    private:
      bool _createDeviceSwapChain(HWND renderHwnd);
      bool _createRenderTargetView();
      bool _createDepthStencilView();
      bool _initializeMatrixes();
      bool _initializeConstantBuffer();
      void _updateWorldConstantBuffer();
      void _updateModelConstantBuffer();
      void _clearContext();
      void _setRenderTargets();

    struct WorldConstantBuffer {
      XMMATRIX mView;
      XMMATRIX mProjection;
    };

    struct ModelConstantBuffer {
      XMMATRIX mWorld;
    };

      ID3D11Device*           _device;
      ID3D11DeviceContext*    _immediateContext;
      IDXGISwapChain*         _swapChain;
      ID3D11RenderTargetView* _renderTargetView;
      ID3D11Texture2D*        _backBuffer;
      ID3D11DepthStencilView* _depthStencilView;
      ID3D11Texture2D*        _depthStencil;
      ID3D11Buffer*           _constantBuffer;

      vector<Material*>       _materials;
      vector<Model*>          _models;

      XMMATRIX                _matrixProjection;
    };

  }

}
#endif // DIRECTX_FACADE_H_