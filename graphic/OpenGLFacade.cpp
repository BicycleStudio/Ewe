#include "OpenGLFacade.h"

static const float sceneColor[4]{ 0.95f, 0.55f, 0.65f, 1.0f };
static const int bitDepth = 16;

using graphic::open_gl::GraphicFacade;

GraphicFacade::GraphicFacade() {
  log = new utils::Logger(typeid(*this).name());
  _hDC = 0;
  _hRC = 0;
}

GraphicFacade::~GraphicFacade() {
  delete log;
}

bool GraphicFacade::_initializeGraphic(int hdc, int sizeX, int sizeY) {
  _hDC = reinterpret_cast<HDC>(hdc);

  if (!(_hRC = wglCreateContext(_hDC))) {
    log->fatal("Can't Create A GL Rendering Context.");
    _shutdown();
    return false;
  }

  if (!wglMakeCurrent(_hDC, _hRC)) {
    log->fatal("Can't Activate The GL Rendering Context.");
    _shutdown();
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

void GraphicFacade::_shutdown() {
  if (_hRC) {
    if (!wglMakeCurrent(NULL, NULL)) 
      log->fatal("Release Of DC And RC Failed.");
    if (!wglDeleteContext(_hRC))
      log->fatal("Release Rendering Context Failed.");
    _hRC = NULL;
  }
}

bool GraphicFacade::_resizeBuffers(int width, int height) {
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

void GraphicFacade::_beginScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
}

void GraphicFacade::_endScene() {
  SwapBuffers(_hDC);
}