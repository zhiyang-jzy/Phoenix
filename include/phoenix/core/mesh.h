//
// Created by jzy99 on 2021/1/21.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_MESH_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_MESH_H_

#include <eigen3/Eigen/Dense>
#include <memory>
#include <vector>
#include<phoenix/core/phoenix.h>

class ImageTexture;

struct Vertex {
  Point3f position;
  Vector3f normal;
  Vector2f texcoord;

  Vertex();

};

class Mesh {
 public:
  vector<Vertex> vertices;
  vector<uint32_t> indices;


 public:
  Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) :
      vertices(std::move(vertices)), indices(std::move(indices)) {}

  Vec2 getTexcood(unsigned int indice,float u,float v) const;

  Mesh();
};


#endif //PHOENIX_INCLUDE_PHOENIX_CORE_MESH_H_
