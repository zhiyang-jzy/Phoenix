//
// Created by jzy99 on 2021/1/21.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_MESH_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_MESH_H_

#include <eigen3/Eigen/Dense>
#include <memory>
#include <vector>
#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>

PHOENIX_NAMESPACE_BEGIN

struct Vertex {
  Point3f position;
  Vector3f normal;
  Vector2f texcoord;

  Vertex()=default;

};

class Mesh {
 public:
  vector<Vertex> vertices;
  vector<uint32_t> indices;


 public:
  Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) :
      vertices(std::move(vertices)), indices(std::move(indices)) {}

  [[nodiscard]] Vector2f getTexcood(unsigned int indice,float u,float v) const{
    auto ind1 = indices[indice*3],ind2 = indices[indice*3+1],ind3 = indices[indice*3+2];
    auto coord1 = vertices[ind1].texcoord,coord2 = vertices[ind2].texcoord,coord3 = vertices[ind3].texcoord;
    return (1-u-v)*coord1+u*coord2+v*coord3;
  }

  Mesh()=default;
};

PHOENIX_NAMESPACE_END


#endif //PHOENIX_INCLUDE_PHOENIX_CORE_MESH_H_
