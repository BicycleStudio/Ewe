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
  if(fp == NULL) {
    log->error("Can't open file " + fileName + " for reading.");
    return false;
  }
  MdlHeader mdlHeader;
  fscanf_s(fp, "%d %d", &mdlHeader.countVerts, &mdlHeader.countFaces);
  
  Vertex* vertexs = new Vertex[mdlHeader.countVerts];

  for(int i = 0; i < mdlHeader.countVerts; i++)
    fscanf_s(fp, "%f %f %f", &vertexs[i].position.x, &vertexs[i].position.y, &vertexs[i].position.z);

  int countInds = mdlHeader.countFaces * 3;
  UINT* indexes = new UINT[countInds];
  for(int i = 0; i < mdlHeader.countFaces; i++)
    fscanf_s(fp, "%d %d %d", &indexes[i * 3], &indexes[i * 3 + 1], &indexes[i * 3 + 2]);
  
  fclose(fp);

  if(!_vertexs.initialize(device, vertexs, sizeof(Vertex), mdlHeader.countVerts)) {
    delete vertexs;
    delete indexes;
    return false;
  }
  if(!_indexs.initialize(device, indexes, countInds)) {
    delete vertexs;
    delete indexes;
    return false;
  }
  delete vertexs;
  delete indexes;
  return true;
}

void Model::shutdown() {
  _vertexs.shutdown();
  _indexs.shutdown();
}