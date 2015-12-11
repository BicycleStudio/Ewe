#include "DInputFacade.h"

#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }
#define CHECK_HRESULT(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }

using io::direct_input::InputFacade;

InputFacade::InputFacade() {
  log = new utils::Logger(typeid(*this).name());
  _device = 0;
  _keyboard = 0;
  _mouse = 0;
}

InputFacade::~InputFacade() {
  delete log;
}

bool InputFacade::_initialize(int hwnd) {
  HWND hwnd_ = reinterpret_cast<HWND> (hwnd);
  
  CHECK_HRESULT(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&_device, NULL), 
    "Can't create Direct Input.");

  CHECK_HRESULT(_device->CreateDevice(GUID_SysMouse, &_mouse, NULL), 
    "Can't create mouse.");
  CHECK_HRESULT(_mouse->SetDataFormat(&c_dfDIMouse), 
    "Can't set mouse Data Format.");
  CHECK_HRESULT(_mouse->SetCooperativeLevel(hwnd_, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND), 
    "Can't set mouse Cooperative Level.");

  CHECK_HRESULT(_device->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL), 
    "Can't create keys.");
  CHECK_HRESULT(_keyboard->SetDataFormat(&c_dfDIKeyboard), 
    "Can't set keys Data Format.");
  CHECK_HRESULT(_keyboard->SetCooperativeLevel(hwnd_, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND), 
    "Can't set keys Cooperative Level.");

  _initialized = true;

  return true;
}

void InputFacade::_shutdown() {
  _initialized = false;

  SAFE_RELEASE(_mouse);
  SAFE_RELEASE(_keyboard);
  SAFE_RELEASE(_device);
}

void InputFacade::_update() {
  HRESULT hres = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
  if (FAILED(hres)) {
    log->warn("update mouse fails.");
    return;
  }
  hres = _keyboard->GetDeviceState(sizeof(_keyState), (LPVOID)&_keyState);
  if (FAILED(hres)) {
    log->warn("update keyboard fails.");
    return;
  }
}

bool InputFacade::_acquire() {
  CHECK_HRESULT(_mouse->Acquire(), "mouse acquire fails.");
  CHECK_HRESULT(_keyboard->Acquire(), "keys acquire fails.");
  return true;
}

bool InputFacade::_pressed(int key) {
  return _keyState[key];
}
