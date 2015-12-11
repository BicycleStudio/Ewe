#ifndef WINDOW_FACADE_H
#define WINDOW_FACADE_H

#include <ThreadManager.h>
#include <Windows.h>
#include <creational\Singleton.h>

#include <Logger.h>

namespace window_facade {

  class WindowFacade : public thread_manager::ThreadSubjectWithKill, public patterns::Singleton<WindowFacade> {
    WindowFacade();
    WindowFacade(const WindowFacade&);
    friend class patterns::Singleton<WindowFacade>;

    utils::Logger* log;

    void _processCommand(command_manager::Command& c);
  public:
    ~WindowFacade();
    command_manager::ID id();
    void stop() override final;
    void start() override final;
    void pause() override final;
    void resume() override final;

    // This methods only for self call! WndProc hack
    // pp_ prefix means pseudoprivate
    void pp_setMinimized(bool);
    bool pp_getMinimized();
    void pp_sendPause();
    void pp_sendResume();
    void pp_sendKill();
    void pp_sendResize(int, int);

  private:
    void _generateCommandProcessors();
    void _sendDestroyAll();

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
    bool _initialized;
  };

}

#endif // WINDOW_FACADE_H

