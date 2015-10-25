#include "WindowFacade.h"

#include <CommandManager.h>
#include <iostream>
using std::cout;
#define __DX_GRAPHIC

static const int windowFacadeSleep = 100;
static const std::string fullScreenString = "Start fullscreen?";
static const std::string fullScreenProblemString = "FullScreen problem. Start in window?";
static const std::string wndClassName = "windowClass";
static const std::string appName = "ïèó-ïèó";
static const int windowWidth = 800;
static const int windowHeight = 600;
static const bool windowed = true;
#define BITZ 32

command_manager::ID window_facade::WindowFacade::id() {
  return command_manager::ID::WINDOW_FACADE;
}
window_facade::WindowFacade* window_facade::WindowFacade::getInstance() {
  static WindowFacade* facade = new WindowFacade();
  return facade;
}
window_facade::WindowFacade::WindowFacade() {
  log = new utils::Logger(typeid(*this).name());

  _name = appName;
  _wndClassName = wndClassName;
  _width = windowWidth;
  _height = windowHeight;
  _hwnd = NULL;
  _fullscreen = !windowed;
  _minimized = false;

  _hDC = 0;
  _hwnd = 0;
}

window_facade::WindowFacade::~WindowFacade() {
  delete log;
}

void window_facade::WindowFacade::_send(command_manager::Command& c) {
  commandManager_->push(c);
}
void window_facade::WindowFacade::pause() {
  this->_paused = true;
}
void window_facade::WindowFacade::resume() {
  this->_paused = false;
}
void window_facade::WindowFacade::stop() {
  log->info("WindowFacade thread was stopped\n");

  _shutdown();
  this->_willStop = true;
}
void window_facade::WindowFacade::processCommand(command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::PAUSE: log->info("WindowFacade pause"); break;
  case CommandType::RESUME: log->info("WindowFacade resume"); break;
  default: break;
  }
  return;
}
void window_facade::WindowFacade::start() {
  log->info("WindowFacade thread was started");

  if (!_initialize()) {
    pp_sendKill();
    return;
  }
  _sendHwnd();

  MSG msg_;

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(windowFacadeSleep);
    std::this_thread::sleep_for(a);

    if (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg_);
      DispatchMessage(&msg_);
    }
    processCommands();
  }
}
void window_facade::WindowFacade::_generateCommandProcessors() {
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
bool window_facade::WindowFacade::_initialize() {
  getInstance()->_generateCommandProcessors();

  HINSTANCE hInstance_ = GetModuleHandle(NULL);

  DWORD		dwExStyle_;
  DWORD		dwStyle_;
  RECT		windowRect_;

  if (MessageBox(NULL, fullScreenString.c_str(), fullScreenString.c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES) {
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
  wc_.lpszClassName = _wndClassName.c_str();

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
    if (ChangeDisplaySettings(&dmScreenSettings_, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      if (MessageBox(NULL, fullScreenProblemString.c_str(), fullScreenProblemString.c_str(), MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
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
  if (!(_hwnd = CreateWindowEx(dwExStyle_,
    _wndClassName.c_str(),
    _name.c_str(),
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
bool window_facade::WindowFacade::_additionalInitialize() {
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
    _shutdown();
    // TODO: Log "WindowFacade: Can't Create A GL Device Context."
    return false;
  }

  if (!(PixelFormat = ChoosePixelFormat(_hDC, &pfd))) {
    _shutdown();
    // TODO: Log "WindowFacade: Can't Find A Suitable PixelFormat."
    return false;
  }

  if (!SetPixelFormat(_hDC, PixelFormat, &pfd)) {
    _shutdown();
    // TODO: Log "WindowFacade: Can't Set The PixelFormat."
    return false;
  }
  return true;
}
void window_facade::WindowFacade::_shutdown() {
  if (_fullscreen) {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(false);
  }
  if (_hDC && !ReleaseDC(_hwnd, _hDC)) {
    // TODO: Log "WindowFacade: Release Device Context Failed."
    ;
    _hDC = NULL;
  }
  if (_hwnd && !DestroyWindow(_hwnd)) {
    log->fatal("DestroyWindow was failed");
    _hwnd = NULL;
  }
  if (!UnregisterClass(_wndClassName.c_str(), GetModuleHandle(NULL))) {
    log->fatal("UnregisterClass was failed");
  }
}
void window_facade::WindowFacade::pp_setMinimized(bool minimized) {
  _minimized = minimized;
}
bool window_facade::WindowFacade::pp_getMinimized() {
  return _minimized;
}
void window_facade::WindowFacade::_sendHwnd() {
  command_manager::Command hwndToGraphic = command_manager::Command(
    command_manager::ID::WINDOW_FACADE, command_manager::ID::GRAPHIC,
    command_manager::CommandType::INITIALIZE);
#if defined(__DX_GRAPHIC)
  hwndToGraphic.args[0] = reinterpret_cast<int>(_hwnd);
#elif defined(__GL_GRAPHIC)
  hwndToGraphic.args[0] = reinterpret_cast<int>(_hDC);
#endif
  hwndToGraphic.args[1] = _width;
  hwndToGraphic.args[2] = _height;
  _send(hwndToGraphic);

  command_manager::Command hwndToIO = command_manager::Command(
    command_manager::ID::WINDOW_FACADE, command_manager::ID::IO,
    command_manager::CommandType::INITIALIZE);
  hwndToIO.args[0] = reinterpret_cast<int>(_hwnd);
  _send(hwndToIO);
}
void window_facade::WindowFacade::pp_sendPause() {
  command_manager::Command commandPause = command_manager::Command(
    command_manager::ID::WINDOW_FACADE,
    command_manager::ID::THREAD_MANAGER,
    command_manager::CommandType::PAUSE);
  _send(commandPause);
}
void window_facade::WindowFacade::pp_sendResume() {
  command_manager::Command commandResume = command_manager::Command(
    command_manager::ID::WINDOW_FACADE,
    command_manager::ID::THREAD_MANAGER,
    command_manager::CommandType::RESUME);
  _send(commandResume);
}
void window_facade::WindowFacade::pp_sendKill() {
  _sendKill();
}
void window_facade::WindowFacade::pp_sendResize(int width, int height) {
  _width = width;
  _height = height;
  command_manager::Command commandResize = command_manager::Command(
    command_manager::ID::WINDOW_FACADE,
    command_manager::ID::GRAPHIC,
    command_manager::CommandType::RESIZE);
  commandResize.args[0] = width;
  commandResize.args[1] = height;
  _send(commandResize);
}