#ifndef DIRECTX_BUFFER_H_
#define DIRECTX_BUFFER_H_

#include "DependenciesDX.h"
#include <Logger.h>
#include "DXVertex.h"

namespace graphic {

  namespace direct_x {

    class Buffer {
      utils::Logger* log;

    public:
      Buffer();
      ~Buffer();

      void shutdown();
      bool initialize(ID3D11Device*, D3D11_BUFFER_DESC&, D3D11_SUBRESOURCE_DATA&);
      bool initialize(ID3D11Device*, UINT* indexes, int countInds);
      bool initialize(ID3D11Device*, Vertex* verts, int vertSize, int countVerts);
      ID3D11Buffer* get();

    private:
      ID3D11Buffer*   _buffer;
    };

  }
}

#endif // DIRECTX_BUFFER_H_