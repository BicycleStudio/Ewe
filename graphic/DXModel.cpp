#include <io.h>

#include "DXModel.h"

using graphic::direct_x::Model;

Model::Model() {
  log = new utils::Logger(typeid(*this).name());
}

Model::~Model() {
  delete log;
}

bool Model::initialize(ID3D11Device* device, string fileName) {
  if(_access(fileName.c_str(), 0) == -1) {
    log->error("File " + fileName + " does not exist.");
    return false;
  }
  FILE* fp;
  fopen_s(&fp, fileName.c_str(), "r");
  MdlHeader* mdlHeader = new MdlHeader();
  fread(mdlHeader, 1, sizeof(MdlHeader), fp);
  
  Vertex* vertexs = new Vertex[mdlHeader->countVerts];

  for(int i = 0; i < mdlHeader->countVerts; i++) {
    fread(&vertexs[i].position.x, 1, sizeof(float), fp);
    fread(&vertexs[i].position.y, 1, sizeof(float), fp);
    fread(&vertexs[i].position.z, 1, sizeof(float), fp);
  }
  _vertexs.initialize(device, vertexs, sizeof(Vertex), mdlHeader->countVerts);

  fclose(fp);
  return true;
}

void Model::shutdown() {
  _vertexs.shutdown();
  _indexs.shutdown();
}