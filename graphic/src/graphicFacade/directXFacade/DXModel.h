#ifndef DIRECTX_MODEL_H_
#define DIRECTX_MODEL_H_

#include "DependenciesDX.h"
#include "DXBuffer.h"
#include <Logger.h>

using std::string;

namespace graphic {

  namespace direct_x {

    struct MdlHeader {
      int  countVerts;
      int  countFaces;
    };
    class Material;
    class Model {
      utils::Logger* log;

    public:
      Model();
      ~Model();

      bool initialize(ID3D11Device*, string fileName);
      void shutdown();
      void draw(ID3D11DeviceContext*, Material*);

    private:
      Buffer    _vertexs;
      Buffer    _indexs;
      UINT      _countIndex;
    };

  }
}

#endif // DIRECTX_MODEL_H_