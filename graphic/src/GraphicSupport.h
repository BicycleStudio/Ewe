#ifndef GRAPHIC_SUPPORT_H_
#define GRAPHIC_SUPPORT_H_

#include <memory>

#include <GraphicData.h>

#include "Dependencies.h"

namespace graphic {

  class GraphicSupport {
  protected:
    std::shared_ptr<data::Object> getObjectByTraceRay (int, int);
  };

}

#endif // GRAPHIC_SUPPORT_H_
