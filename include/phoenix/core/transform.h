//
// Created by jzy99 on 2021/1/21.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_TRANSFORM_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_TRANSFORM_H_


#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>


PHOENIX_NAMESPACE_BEGIN


struct Transform {
 public:
     
  Transform() :
      m_transform(Eigen::Matrix4f::Identity()),
      m_inverse(Eigen::Matrix4f::Identity()) { }

     
  Transform(const Eigen::Matrix4f &trafo);

     
  Transform(const Eigen::Matrix4f &trafo, const Eigen::Matrix4f &inv)
      : m_transform(trafo), m_inverse(inv) { }

     
  const Eigen::Matrix4f &getMatrix() const {
    return m_transform;
  }

     
  const Eigen::Matrix4f &getInverseMatrix() const {
    return m_inverse;
  }

     
  Transform inverse() const {
    return Transform(m_inverse, m_transform);
  }

     
  Transform operator*(const Transform &t) const;

     
  Vector3f operator*(const Vector3f &v) const {
    return m_transform.topLeftCorner<3,3>() * v;
  }

     
  Normal3f operator*(const Normal3f &n) const {
    return m_inverse.topLeftCorner<3, 3>().transpose() * n;
  }

     
  Point3f operator*(const Point3f &p) const {
    Vector4f result = m_transform * Vector4f(p[0], p[1], p[2], 1.0f);
    return result.head<3>() / result.w();
  }


     
  std::string toString() const;
 private:
  Eigen::Matrix4f m_transform;
  Eigen::Matrix4f m_inverse;
};



PHOENIX_NAMESPACE_END


#endif //PHOENIX_INCLUDE_PHOENIX_CORE_TRANSFORM_H_
