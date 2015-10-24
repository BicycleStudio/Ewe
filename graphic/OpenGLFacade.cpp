#include "OpenGLFacade.h"

static const float sceneColor[4]{ 0.95f, 0.55f, 0.65f, 1.0f };

graphic::OpenGLFacade::OpenGLFacade() {
  _hDC = 0;
  _hRC = 0;
  _hWnd = 0;
}

bool graphic::OpenGLFacade::_initializeGraphic(int hwnd, int sizeX, int sizeY) {
  int PixelFormat;
  _hWnd = reinterpret_cast<HWND> (hwnd);
  // TODO: to config?? i think it must always be 32. or 16. ??
  int bits = 16;

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,                    // Version Number
    PFD_DRAW_TO_WINDOW |  // Format Must Support Window
    PFD_SUPPORT_OPENGL |  // Format Must Support OpenGL
    PFD_DOUBLEBUFFER,     // Must Support Double Buffering
    PFD_TYPE_RGBA,        // Request An RGBA Format
    bits,                 // Select Our Color Depth
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

  if (!(_hDC = GetDC(_hWnd))) {
    _shutdown();
    // TODO: Log "Graphic: Can't Create A GL Device Context."
    return false;
  }

  if (!(PixelFormat = ChoosePixelFormat(_hDC, &pfd))) {
    _shutdown();
    // TODO: Log "Graphic: Can't Find A Suitable PixelFormat."
    return false;
  }

  if (!SetPixelFormat(_hDC, PixelFormat, &pfd))	{
    _shutdown();
    // TODO: Log "Graphic: Can't Set The PixelFormat."
    return false;
  }

  if (!(_hRC = wglCreateContext(_hDC))) {
    _shutdown();
    // TODO: Log "Graphic: Can't Create A GL Rendering Context."
    return false;
  }

  if (!wglMakeCurrent(_hDC, _hRC)) {
    _shutdown();
    // TODO: Log "Graphic: Can't Activate The GL Rendering Context."
    return false;
  }

  glShadeModel(GL_SMOOTH);
  glClearColor(sceneColor[0], sceneColor[1], sceneColor[2], sceneColor[3]);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  return true;
}

void graphic::OpenGLFacade::_shutdown() {
  if (_hRC) {
    if (!wglMakeCurrent(NULL, NULL))
      ;
      // TODO: Log "Graphic: Release Of DC And RC Failed."
    if (!wglDeleteContext(_hRC))
      ;
      // TODO: Log "Graphic: Release Rendering Context Failed."
    _hRC = NULL;
  }
  if (_hDC && !ReleaseDC(_hWnd, _hDC)) {
    // TODO: Log "Graphic: Release Device Context Failed."
    ;
    _hDC = NULL;
  }
}

bool graphic::OpenGLFacade::_resizeBuffers(int width, int height) {
  if (width == 0) width = 1;
  if (height == 0) height = 1;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  return true;
}
void graphic::OpenGLFacade::_beginScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
}
void graphic::OpenGLFacade::_endScene() {
  SwapBuffers(_hDC);
}