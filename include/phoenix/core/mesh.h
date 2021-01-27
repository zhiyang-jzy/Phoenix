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
#include<phoenix/core/dpdf.h>
#include<phoenix/core/sampledata.h>

PHOENIX_NAMESPACE_BEGIN

struct Vertex {
  Point3f position;
  Vector3f normal;
  Vector2f texcoord;

  Vertex() = default;

};

class Mesh {
 public:
  vector<Vertex> vertices;
  vector<uint32_t> indices;
  float area_;
  DiscretePDF dpdf_;

 public:
  Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) :
      vertices(std::move(vertices)), indices(std::move(indices)) {}
  void CalcArea();

  [[nodiscard]] Vector2f getTexcood(unsigned int indice, float u, float v) const {
    auto ind1 = indices[indice * 3], ind2 = indices[indice * 3 + 1], ind3 = indices[indice * 3 + 2];
    auto coord1 = vertices[ind1].texcoord, coord2 = vertices[ind2].texcoord, coord3 = vertices[ind3].texcoord;
    return (1 - u - v) * coord1 + u * coord2 + v * coord3;
  }

  SampleData SamplePoint(Point2f &sample) const {
    auto index = dpdf_.Sample(sample.x());
    index *= 3;

    float alpha = 1 - sqrt(1 - sample.x());
    float beta = sample.y() * sqrt(1 - sample.x());
    float theta = 1 - alpha - beta;

    Point3f a(vertices[indices[index]].position), b(vertices[indices[index + 1]].position),
        c(vertices[indices[index + 2]].position);

    SampleData res;
    res.point = a * alpha + b * beta + c * theta;
    res.pdf = 1.0f / dpdf_.GetSum();
    res.normal = ((a - b).cross(a - c)).normalized();

    return res;
  }

  Mesh() = default;
};
inline void Mesh::CalcArea() {
  area_ = 0;
  dpdf_.Clear();
  auto vertex_count = indices.size();
  for (int i = 0; i < vertex_count; i += 3) {
    auto a = vertices[indices[i]].position, b = vertices[indices[i + 1]].position,
        c = vertices[indices[i + 2]].position;
    float area = (c - a).cross(b - a).norm() * 0.5f;
    area_ += area;
    dpdf_.Append(area);
  }
  dpdf_.normalize();
}

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_MESH_H_
