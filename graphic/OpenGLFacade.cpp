#include "OpenGLFacade.h"

static const float sceneColor[4]{ 0.95f, 0.55f, 0.65f, 1.0f };
static const int bitDepth = 16;

graphic::OpenGLFacade::OpenGLFacade() {
  _hDC = 0;
  _hRC = 0;
  _hWnd = 0;
}

bool graphic::OpenGLFacade::_initializeGraphic(int hwnd, int sizeX, int sizeY) {

  glShadeModel(GL_SMOOTH);
  glClearColor(sceneColor[0], sceneColor[1], sceneColor[2], sceneColor[3]);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  return true;
}

void graphic::OpenGLFacade::_shutdown() {

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