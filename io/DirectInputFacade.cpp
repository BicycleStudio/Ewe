#include "DirectInputFacade.h"

// Again this macro ...
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }
// New macro )) i think, nice
#define CHECK_HRESULT(hres,msg) { if(FAILED(hres)) { /* TODO: log msg*/ return false; } }

io::DirectInputFacade::DirectInputFacade() {
  _device = 0;
  _keyboard = 0;
  _mouse = 0;
}

bool io::DirectInputFacade::_initialize(int hwnd) {
  HWND hwnd_ = reinterpret_cast<HWND> (hwnd);
  
  CHECK_HRESULT(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&_device, NULL), 
    "IO: Can't create Direct Input.");

  CHECK_HRESULT(_device->CreateDevice(GUID_SysMouse, &_mouse, NULL), 
    "IO: Can't create mouse.");
  CHECK_HRESULT(_mouse->SetDataFormat(&c_dfDIMouse), 
    "IO: Can't set mouse Data Format.");
  CHECK_HRESULT(_mouse->SetCooperativeLevel(hwnd_, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND), 
    "IO: Can't set mouse Cooperative Level.");

  CHECK_HRESULT(_device->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL), 
    "IO: Can't create keys.");
  CHECK_HRESULT(_keyboard->SetDataFormat(&c_dfDIKeyboard), 
    "IO: Can't set keys Data Format.");
  CHECK_HRESULT(_keyboard->SetCooperativeLevel(hwnd_, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND), 
    "IO: Can't set keys Cooperative Level.");

  _initialized = true;

  return true;
}

void io::DirectInputFacade::_shutdown() {
  SAFE_RELEASE(_mouse);
  SAFE_RELEASE(_keyboard);
  SAFE_RELEASE(_device);

  _initialized = false;
}

bool io::DirectInputFacade::_update() {
  CHECK_HRESULT(_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState), "IO: update mouse fails.");
  CHECK_HRESULT(_keyboard->GetDeviceState(sizeof(_keyState), (LPVOID)&_keyState), "IO: update keys fails.");

  return true;
}

bool io::DirectInputFacade::_acquire() {
  CHECK_HRESULT(_mouse->Acquire(), "IO: mouse acquire fails.");
  CHECK_HRESULT(_keyboard->Acquire(), "IO: keys acquire fails.");

  return true;
}

bool io::DirectInputFacade::_pressed(int key) {
  return _keyState[key];
}
