#ifndef WINDOW_FACADE_H
#define WINDOW_FACADE_H

#include <ThreadManager.h>
#include <Windows.h>

#include <Logger.h>

namespace window_facade {

  class WindowFacade : public thread_manager::ThreadSubjectWithKill {
    utils::Logger* log;

    void processCommand(command_manager::Command& c);
  public:
    static WindowFacade* getInstance();
    ~WindowFacade();
    command_manager::ID id();
    void stop();
    void start();
    void pause();
    void resume();

    // This methods only for self call! WndProc hack
    // pp_ prefix means pseudoprivate
    void pp_setMinimized(bool);
    bool pp_getMinimized();
    void pp_sendPause();
    void pp_sendResume();
    void pp_sendKill();
    void pp_sendResize(int, int);

  private:
    WindowFacade();
    WindowFacade(WindowFacade&);
    void _generateCommandProcessors();

  private:
    void _sendHwnd();
    void _send(command_manager::Command& c);
    bool _initialize();
    bool _additionalInitialize();
    void _shutdown();

    std::map<UINT, std::function<bool(WPARAM, LPARAM)>> _commandProcessors;
    HWND  _hwnd;
    HDC   _hDC;
    int   _width;
    int   _height;

    std::string _name;
    std::string _wndClassName;
    bool _fullscreen;
    bool _minimized;
  };

}

#endif // WINDOW_FACADE_H

