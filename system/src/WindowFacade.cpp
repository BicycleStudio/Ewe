#include "WindowFacade.h"
#include <Utils.h>
#include <CommandManager.h>
#include <iostream>

#include <string>

#define __DX_GRAPHIC

using namespace window_facade;
using namespace command_manager;
using namespace utils;
using namespace std;

static const int windowFacadeSleep = 100;
static const std::string fullScreenString = "Start fullscreen?";
static const std::string fullScreenProblemString = "FullScreen problem. Start in window?";
static const std::string wndClassName = "windowClass";
static const std::string appName = "ïèó-ïèó";
static const int windowWidth = 800;
static const int windowHeight = 600;
static const bool windowed = true;
#define BITZ 32

ID WindowFacade::id() {
  return ID::WINDOW_FACADE;
}

WindowFacade::WindowFacade() {
  log = new utils::Logger(typeid(*this).name());

  _name = appName;
  _wndClassName = wndClassName;
  _width = windowWidth;
  _height = windowHeight;
  _hwnd = NULL;
  _fullscreen = !windowed;
  _minimized = false;
  _sleepThread = windowFacadeSleep;

  _hDC = 0;
  _hwnd = 0;
}

WindowFacade::~WindowFacade() {
  delete log;
}

void WindowFacade::_send(command_manager::Command& c) {
  _commandManager->push(c);
}

void WindowFacade::pause() {
  log->info("thread was paused\n");
  this->_paused = true;
}

void WindowFacade::resume() {
  log->info("thread was resumed\n");
  this->_paused = false;
}

void WindowFacade::stop() {
  log->info("thread was stopped\n");

  _shutdown();
  this->_willStop = true;
}

void WindowFacade::_processCommand(command_manager::Command& c) {
  switch (c.commandType) {
  case KILL: 
    _shutdown();
    _sendDestroyAll();
    break;
  default: break;
  }
  return;
}

bool WindowFacade::initialize() {
  if(!_initialize()) {
    pp_sendKill();
    return false;
  }
  _initialized = true;
  _sendInitialize();
  return true;
}

void WindowFacade::processTick() {
  MSG msg_;
  if(PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg_);
    DispatchMessage(&msg_);
  }
  _processCommands();
}

void WindowFacade::_generateCommandProcessors() {
  _commandProcessors[WM_CLOSE] = [](WPARAM wParam, LPARAM lParam) {
    WindowFacade::getInstance()->pp_sendKill();
    return true;
  };
  _commandProcessors[WM_SIZE] = [](WPARAM wParam, LPARAM lParam) {
    WindowFacade::getInstance()->pp_sendResize(LOWORD(lParam), HIWORD(lParam));
    return true;
  }; 
  _commandProcessors[WM_SYSCOMMAND] = [](WPARAM wParam, LPARAM lParam) {
    switch (wParam)  {
    case SC_MINIMIZE:
      WindowFacade::getInstance()->pp_setMinimized(true);
      WindowFacade::getInstance()->pp_sendPause();
      break;
    case SC_RESTORE:  
      WindowFacade::getInstance()->pp_setMinimized(false);
      break;
    case SC_SCREENSAVE:
    case SC_MONITORPOWER: return true;
    default: return false;
    }
    return false;
  };
  _commandProcessors[WM_ACTIVATE] = [](WPARAM wParam, LPARAM lParam) {
    if (!WindowFacade::getInstance()->pp_getMinimized()) {
      if (wParam) WindowFacade::getInstance()->pp_sendResume();
      else        WindowFacade::getInstance()->pp_sendPause();
      return true;
    }
    return false;
  };
}

bool WindowFacade::_initialize() {
  getInstance()->_generateCommandProcessors();

  HINSTANCE hInstance_ = GetModuleHandle(NULL);

  DWORD		dwExStyle_;
  DWORD		dwStyle_;
  RECT		windowRect_;
   
  wstring wfullscreen;
  wfullscreen %= fullScreenString;
  if(MessageBox(NULL, wfullscreen.c_str(), wfullscreen.c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES) {
    _fullscreen = true;
    HDC hDC_ = GetDC(NULL);
    _width = GetDeviceCaps(hDC_, HORZRES);
    _height = GetDeviceCaps(hDC_, VERTRES);
    ReleaseDC(NULL, hDC_);
  }

  windowRect_.left = (long)0;
  windowRect_.right = (long)_width;
  windowRect_.top = (long)0;
  windowRect_.bottom = (long)_height;

  WNDPROC wndproc_ = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT {
    auto& cp = window_facade::WindowFacade::getInstance()->_commandProcessors;
    auto& it = cp.find(uMsg);
    if (it != cp.end()) {
      if(it->second(wParam, lParam)) return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  };

  wstring wwndclass;
  wwndclass %= _wndClassName;

  WNDCLASS	wc_;
  wc_.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc_.lpfnWndProc = wndproc_;
  wc_.cbClsExtra = 0;
  wc_.cbWndExtra = 0;
  wc_.hInstance = hInstance_;
  wc_.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc_.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc_.hbrBackground = NULL;
  wc_.lpszMenuName = NULL;
  wc_.lpszClassName = wwndclass.c_str();

  if (!RegisterClass(&wc_)) {
    log->fatal("RegisterClass was failed");
    return false;
  }
  if (_fullscreen) {
    DEVMODE dmScreenSettings_;
    memset(&dmScreenSettings_, 0, sizeof(dmScreenSettings_));
    dmScreenSettings_.dmSize = sizeof(dmScreenSettings_);
    dmScreenSettings_.dmPelsWidth = _width;
    dmScreenSettings_.dmPelsHeight = _height;
    dmScreenSettings_.dmBitsPerPel = BITZ;
    dmScreenSettings_.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    if(ChangeDisplaySettings(&dmScreenSettings_, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      wstring wfullscreenError;
      wfullscreenError %= fullScreenProblemString;
      if(MessageBox(NULL, wfullscreenError.c_str(), wfullscreenError.c_str(), MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
        _fullscreen = false;
        _width = windowWidth;
        _height = windowHeight;
        windowRect_.right = (long)_width;
        windowRect_.bottom = (long)_height;
      }
      else {
        log->fatal("ChangeDisplaySettings was failed");
        return false;
      }
    }
  }
  if (_fullscreen) {
    dwExStyle_ = WS_EX_APPWINDOW;
    dwStyle_ = WS_POPUP;
    ShowCursor(FALSE);
  }
  else {
    dwExStyle_ = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle_ = WS_OVERLAPPEDWINDOW;
  }
  AdjustWindowRectEx(&windowRect_, dwStyle_, FALSE, dwExStyle_);
  wstring wname; wname %= _name;
  if(!(_hwnd = CreateWindowEx(dwExStyle_,
    wwndclass.c_str(),
    wname.c_str(),
    dwStyle_ |
    WS_CLIPSIBLINGS |
    WS_CLIPCHILDREN,
    0, 0,
    windowRect_.right - windowRect_.left,
    windowRect_.bottom - windowRect_.top,
    NULL,
    NULL,
    hInstance_,
    NULL))) {
    _shutdown();
    log->fatal("CreateWindowEx was failed");
    return false;
  }
#ifdef __GL_GRAPHIC
  _additionalInitialize();
#endif
  ShowWindow(_hwnd, SW_SHOW);
  SetForegroundWindow(_hwnd);
  SetFocus(_hwnd);

  return true;
}

bool WindowFacade::_additionalInitialize() {
  int PixelFormat;

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,                    // Version Number
    PFD_DRAW_TO_WINDOW |  // Format Must Support Window
    PFD_SUPPORT_OPENGL |  // Format Must Support OpenGL
    PFD_DOUBLEBUFFER,     // Must Support Double Buffering
    PFD_TYPE_RGBA,        // Request An RGBA Format
    BITZ,                 // Select Our Color Depth
    0, 0, 0, 0, 0, 0,     // Color Bits Ignored
    0,                    // No Alpha Buffer
    0,                    // Shift Bit Ignored
    0,                    // No Accumulation Buffer
    0, 0, 0, 0,           // Accumulation Bits Ignored
    16,                   // 16Bit Z-Buffer (Depth Buffer)  
    0,                    // No Stencil Buffer
    0,                    // No Auxiliary Buffer
    PFD_MAIN_PLANE,       // Main Drawing Layer
    0,                    // Reserved
    0, 0, 0               // Layer Masks Ignored
  };

  if (!(_hDC = GetDC(_hwnd))) {
    log->fatal("Can't Create A GL Device Context.");
    _shutdown();
    return false;
  }

  if (!(PixelFormat = ChoosePixelFormat(_hDC, &pfd))) {
    log->fatal("Can't Find A Suitable PixelFormat.");
    _shutdown();
    return false;
  }

  if (!SetPixelFormat(_hDC, PixelFormat, &pfd)) {
    log->fatal("Can't Set The PixelFormat.");
    _shutdown();
    return false;
  }
  return true;
}

void WindowFacade::_shutdown() {
  if (!_initialized) return;
  _initialized = false;

  if (_hwnd) {
    if (_hDC && !ReleaseDC(_hwnd, _hDC)) {
      log->fatal("Release Device Context Failed.");
      _hDC = NULL;
    }
   if(!DestroyWindow(_hwnd)) 
      log->fatal("DestroyWindow was failed");
   _hwnd = NULL;
  }
  wstring wwndclass; wwndclass %= _wndClassName;
  if(!UnregisterClass(wwndclass.c_str(), GetModuleHandle(NULL))) {
    log->fatal("UnregisterClass was failed");
  }

  if (_fullscreen) {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(true);
  }
}

void WindowFacade::pp_setMinimized(bool minimized) {
  this->_minimized = minimized;
}

bool WindowFacade::pp_getMinimized() {
  return this->_minimized;
}

void WindowFacade::_sendInitialize() {
  Command hwndToGraphic = Command(this->id(), ID::GRAPHIC, INITIALIZE);
#if defined(__DX_GRAPHIC)
  hwndToGraphic.argInt.push_back(reinterpret_cast<int>(_hwnd));
#elif defined(__GL_GRAPHIC)
  hwndToGraphic.args[0] = reinterpret_cast<int>(_hDC);
#endif
  hwndToGraphic.argInt.push_back(this->_width);
  hwndToGraphic.argInt.push_back(this->_height);
  _send(hwndToGraphic);

  Command hwndToIO = Command(this->id(), ID::IO, INITIALIZE);
  hwndToIO.argInt.push_back(reinterpret_cast<int>(_hwnd));
  _send(hwndToIO);

  Command hwndToSound = Command(this->id(), ID::SOUND, INITIALIZE);
  hwndToSound.argInt.push_back(reinterpret_cast<int>(_hwnd));
  _send(hwndToSound);

  _send(Command(this->id(), ID::WINDOW_FACADE, INITIALIZE));
  _send(Command(this->id(), ID::LOGIC, INITIALIZE));
}

void WindowFacade::pp_sendPause() {
  Command commandPause = Command(this->id(), ID::THREAD_MANAGER, PAUSE);
  _send(commandPause);
}

void WindowFacade::pp_sendResume() {
  Command commandResume = Command(this->id(), ID::THREAD_MANAGER, RESUME);
  _send(commandResume);
}

void WindowFacade::pp_sendKill() {
  _sendKill();
}

void WindowFacade::pp_sendResize(int width, int height) {
  this->_width = width;
  this->_height = height;
  Command commandResize = Command(this->id(), ID::GRAPHIC, RESIZE);
  commandResize.argInt.push_back(this->_width);
  commandResize.argInt.push_back(this->_height);
  _send(commandResize);
}

void WindowFacade::_sendDestroyAll() {
  Command cmndRealKill = Command(this->id(), ID::THREAD_MANAGER, DESTROY_ALL);
  _commandManager->push(cmndRealKill);
}