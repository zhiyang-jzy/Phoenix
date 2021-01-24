//
// Created by jzy99 on 2021/1/21.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_FRAM_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_FRAM_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>

PHOENIX_NAMESPACE_BEGIN

inline void CoordinateSystem(const Vector3f &a, Vector3f &b, Vector3f &c) {
  if (std::abs(a.x()) > std::abs(a.y())) {
    float invLen = 1.0f / std::sqrt(a.x() * a.x() + a.z() * a.z());
    c = Vector3f(a.z() * invLen, 0.0f, -a.x() * invLen);
  } else {
    float invLen = 1.0f / std::sqrt(a.y() * a.y() + a.z() * a.z());
    c = Vector3f(0.0f, a.z() * invLen, -a.y() * invLen);
  }
  b = c.cross(a);
}

class Frame {
 public:
  Vector3f s, t;
  Normal3f n;
 public:
  explicit Frame(Vector3f n):n(n) {
    CoordinateSystem(n, s, t);
  }
  Frame() = default;
  Vector3f ToLocal(const Vector3f &v) const {
    return Vector3f(
        v.dot(s), v.dot(t), v.dot(n)
    );
  }

  /// Convert from local coordinates to world coordinates
  Vector3f ToWorld(const Vector3f &v) const {
    return s * v.x() + t * v.y() + n * v.z();
  }

};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_FRAM_H_
