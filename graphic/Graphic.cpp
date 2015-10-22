#include "Graphic.h"

#include <iostream>

#include <CommandManager.h>

using std::cout;
static const int graphicSleep = 100;
// need for correct use dx in big sizes
static const int veryBigSize = 2000;

command_manager::ID graphic::Graphic::id() {
  return command_manager::ID::GRAPHIC;
}

graphic::Graphic::Graphic ( ) {
  _device = 0;
  _immediateContext = 0;
  _swapChain = 0;
}

void graphic::Graphic::stop() {
  cout << "Graphic thread was stopped\n";
  _shutdown();

  this->willStop = true;
}

void graphic::Graphic::processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) { 
  case CommandType::INITIALIZE: 
    cout << "Graphic init\thwnd = " << c.args[0] << "\n"; 
    if (!_initialize(reinterpret_cast<HWND> (c.args[0]))) _sendKill();
    cout << "Graphic init [OK]\n";
    break;
  case CommandType::PAUSE: cout << "Graphic pause"; break;
  case CommandType::RESUME: cout << "Graphic resume"; break;
  case CommandType::RESIZE: cout << "Graphic resize\tx = " << c.args[0] << " y = " << c.args[1] << "\n"; break;
  default: break;
  }
  return;
}

void graphic::Graphic::start() {
  cout << "Graphic thread was started\n";

  while (!this->willStop) {
    auto a = std::chrono::milliseconds(graphicSleep);
    std::this_thread::sleep_for (a);

    processCommands ( );
  }
}

bool graphic::Graphic::_initialize(HWND renderHwnd) {
  // a little hack. first init of dx must be very BIG
  if (!_createDeviceSwapChain(renderHwnd)) return false;
  // 0. resize swapChain to correct size

  // TODO: 
  // 1. create render target view
  // 2. create depth stencil view & depth stencil states { enable, disable ( for techniques like skybox ) }
  // 3. create rasterizer states { Solid, SolidNonCull( transparent objects ), Wireframe }
  // 4. create sampler states { Linear, ForDeffered }
  // 5. init all shaders

  // 6. set view port & render targets
  // 7. set projection matrix

  return true;
}
bool graphic::Graphic::_createDeviceSwapChain(HWND renderHwnd) {
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
  sd.BufferDesc.Width = veryBigSize;
  sd.BufferDesc.Height = veryBigSize;
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
    // TODO: Log "DX 11 can't initialize on this machine"
    return false; 
  }
  return true;
}
void graphic::Graphic::_sendKill() {
  command_manager::Command cmd = command_manager::Command(
    id(), command_manager::ID::THREAD_MANAGER,
    command_manager::CommandType::KILL);
  commandManager_->push(cmd);
}

void graphic::Graphic::_shutdown() {
  if (_swapChain) { 
    _swapChain->Release(); 
    _swapChain = 0; 
  }
  if (_immediateContext) { 
    _immediateContext->Release(); 
    _immediateContext = 0; 
  }
  if (_device) { 
    _device->Release(); 
    _device = 0; 
  }
}