#include "DirectXFacade.h"

#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }
static const float sceneColor[4]{ 0.5f, 0.75f, 0.85f, 1.0f };

graphic::DirectXFacade::DirectXFacade() { 
  _device = 0;
  _immediateContext = 0;
  _swapChain = 0;

  _renderTargetView = 0;
  _backBuffer = 0;
  _depthStencilView = 0;
  _depthStencil = 0;
}

bool graphic::DirectXFacade::_initializeGraphic(int hwnd, int sizeX, int sizeY) {
  _sizeX = sizeX;
  _sizeY = sizeY;

  if (!_createDeviceSwapChain(reinterpret_cast<HWND> (hwnd))) return false;
  if (!_resizeBuffers(sizeX, sizeY)) return false;

  // TODO: 
  // 3. create rasterizer states { Solid, SolidNonCull( transparent objects ), Wireframe }
  // 4. create sampler states { Linear, ForDeffered }
  // 5. init all shaders

  _setRenderTargets();
  // 6. set projection matrix

  return true;
}

bool graphic::DirectXFacade::_createDeviceSwapChain(HWND renderHwnd) {
  HRESULT hr;
  UINT createDeviceFlags = 0;

  D3D_FEATURE_LEVEL       featureLevel_;
  D3D_DRIVER_TYPE         driverType_;

#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  D3D_DRIVER_TYPE driverTypes[] =
  {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE(driverTypes);

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE(featureLevels);
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  // First init with bigSize, after call resize with correct size
  sd.BufferDesc.Width = _sizeX;
  sd.BufferDesc.Height = _sizeY;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = renderHwnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
    driverType_ = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain(NULL, driverType_, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
      D3D11_SDK_VERSION, &sd, &_swapChain, &_device, &featureLevel_, &_immediateContext);
    if (SUCCEEDED(hr))			break;
  }
  if (FAILED(hr)) {
    // TODO: Log "Graphic: DX 11 can't initialize on this machine"
    return false;
  }
  return true;
}

void graphic::DirectXFacade::_shutdown() {
  if (_initialized)
    _clearContext();
  _initialized = false;
  SAFE_RELEASE(_swapChain);
  SAFE_RELEASE(_immediateContext);
  SAFE_RELEASE(_device);
}

void graphic::DirectXFacade::_clearContext() {
  _immediateContext->ClearState();
  _immediateContext->OMSetRenderTargets(0, NULL, NULL);
  SAFE_RELEASE(_renderTargetView);
  SAFE_RELEASE(_backBuffer);
  SAFE_RELEASE(_depthStencilView);
  SAFE_RELEASE(_depthStencil);
}

void graphic::DirectXFacade::_setRenderTargets() {
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)_sizeX;
  vp.Height = (FLOAT)_sizeY;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  _immediateContext->RSSetViewports(1, &vp);
  _immediateContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
}

bool graphic::DirectXFacade::_createRTV() {
  HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&_backBuffer);
  if (FAILED(hr)) {
    // TODO: Log "Graphic: can't get buffer from swapChain!"
    return false;
  }
  hr = _device->CreateRenderTargetView(_backBuffer, NULL, &_renderTargetView);
  if (FAILED(hr)) {
    // TODO: Log "Graphic: can't create renderTargetView from backBuffer!"
    return false;
  }
  return true;
}

bool graphic::DirectXFacade::_createDSV() {
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory(&descDepth, sizeof(descDepth));
  descDepth.Width = _sizeX;	descDepth.Height = _sizeY;
  descDepth.MipLevels = 1;	descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = 1;	descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;	descDepth.MiscFlags = 0;
  HRESULT hr = _device->CreateTexture2D(&descDepth, NULL, &_depthStencil);
  if (FAILED(hr)) {
    // TODO: Log "Graphic: can't create depthStencil!"
    return false;
  }

  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory(&descDSV, sizeof(descDSV));	descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	descDSV.Texture2D.MipSlice = 0;
  hr = _device->CreateDepthStencilView(_depthStencil, &descDSV, &_depthStencilView);
  if (FAILED(hr)) {
    // TODO: Log "Graphic: can't create depthStencilView from depthStencil!"
    return false;
  }
  return true;
}

void graphic::DirectXFacade::_beginScene() {
  _immediateContext->ClearRenderTargetView(_renderTargetView, sceneColor);
  _immediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void graphic::DirectXFacade::_endScene() {
  _swapChain->Present(0, 0);
}

bool graphic::DirectXFacade::_resizeBuffers(int sizeX, int sizeY) {
  _sizeX = sizeX; _sizeY = sizeY;
  if (_sizeX == 0) _sizeX = 1;
  if (_sizeY == 0) _sizeY = 1;
  _clearContext();

  HRESULT hr = _swapChain->ResizeBuffers(1, _sizeX, _sizeY, DXGI_FORMAT_UNKNOWN, 0);
  if (FAILED(hr)) {
    // TODO: Log "Graphic: can't resize buffers!"
    return false;
  }

  if (!_createRTV()) return false;
  if (!_createDSV()) return false;

  _setRenderTargets();

  // TODO: set projection matrix
  return true;
}