
#pragma once
#include<string>
#include <utility>
#include<vector>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include <eigen3/Eigen/Dense>
#include <unordered_map>
#include "mesh.h"

PHOENIX_NAMESPACE_BEGIN

class Model {
 public:
  std::vector<shared_ptr<Mesh> > meshes;

 public:
  Model()=default;
  void Load(const string& path);

 private:
  void processNode(aiNode* node, const aiScene* scene);
  shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

};

PHOENIX_NAMESPACE_END