//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/transform.h>

PHOENIX_NAMESPACE_BEGIN

Transform::Transform(const Eigen::Matrix4f &trafo)
    : m_transform(trafo), m_inverse(trafo.inverse()) {}

Transform Transform::operator*(const Transform &t) const {
  return Transform(m_transform * t.m_transform,
                   t.m_inverse * m_inverse);
}



PHOENIX_NAMESPACE_END

