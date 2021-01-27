//
// Created by jzy99 on 2021/1/21.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLING_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLING_H_

#include<phoenix/core/phoenix.h>

PHOENIX_NAMESPACE_BEGIN

inline Point2f SquareToUniformSquare(const Point2f &sample) {
  return sample;
}

inline float SquareToUniformSquarePdf(const Point2f &sample) {
  return ((sample.array() >= 0).all() && (sample.array() <= 1).all()) ? 1.0f : 0.0f;
}

inline Point2f SquareToTent(const Point2f &sample) {

  float x = sample.x() <= 0.5 ? sqrt(2 * sample.x()) - 1 : 1 - sqrt(2 * (1 - sample.x()));
  float y = sample.y() <= 0.5 ? sqrt(2 * sample.y()) - 1 : 1 - sqrt(2 * (1 - sample.y()));
  return Point2f(x, y);
  /*throw NoriException("Warp::squareToTent() is not yet implemented!");*/
}

inline float SquareToTentPdf(const Point2f &p) {
  /*throw NoriException("Warp::squareToTentPdf() is not yet implemented!");*/
  //return ((p.array() >= -1).all() && (p.array() <= 1).all()) ? 0.25*(1-abs(p.x()))* (1 - abs(p.y())) : 0.0f;
  //return ((p.array() >= -1).all() && (p.array() <= 1).all()) ? 0.25 : 0.0f;

  return ((p.array() >= -1).all() && (p.array() <= 1).all()) ? (1 - abs(p.x())) * (1 - abs(p.y())) : 0.0f;
}

inline Point2f SquareToUniformDisk(const Point2f &sample) {
  /*throw NoriException("Warp::squareToUniformDisk() is not yet implemented!");*/
  /*return sample * 2 - Point2f(1, 1);*/
  Point2f a(sin(sample.x() * 2 * PI), cos(sample.x() * 2 * PI));
  return sqrt(sample.y()) * a;
}

inline float SquareToUniformDiskPdf(const Point2f &p) {
  /*throw NoriException("Warp::squareToUniformDiskPdf() is not yet implemented!");*/

  return p.squaredNorm() <= 1.0 ? INV_PI : 0;
  //p.norm()
  //if(p.squaredNorm()<=1)
  //return INV_PI;
}

inline Vector3f SquareToUniformSphere(const Point2f &sample) {
  /*throw NoriException("Warp::squareToUniformSphere() is not yet implemented!");*/
  Point2f a = SquareToUniformDisk(sample);

  float r2 = a.squaredNorm();

  float x = 2 * a.x() * sqrt(1 - r2);
  float y = 2 * a.y() * sqrt(1 - r2);
  float z = 1 - 2 * r2;

  return Vector3f(x, y, z);

}

inline float SquareToUniformSpherePdf(const Vector3f &v) {
  /*throw NoriException("Warp::squareToUniformSpherePdf() is not yet implemented!");*/

  return 0.25f * INV_PI;
}

inline Vector3f SquareToUniformHemisphere(const Point2f &sample) {
  double theta = 2 * PI * sample[0];
  double phi = acos(1 - sample[1]);

  return Vector3f(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
}

inline float SquareToUniformHemispherePdf(const Vector3f &v) {
  /*throw NoriException("Warp::squareToUniformHemispherePdf() is not yet implemented!");*/

  if(sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) > 1 || v[2] < 0)
    return 0.f;

  return 1.f/(2*PI);
}

inline Vector3f SquareToCosineHemisphere(const Point2f &sample) {
  float theta = acos(2 * sample.x() - 1) / 2;
  float z = cos(theta);
  float r = sqrtf(1.f - z * z);
  float phi = 2 * PI * sample.y();
  return Vector3f(r * cos(phi), r * sin(phi), z);
}

inline float SquareToCosineHemispherePdf(const Vector3f &v) {
  return v.z() > 0 ? v.z() / PI : 0.f;
}

inline Vector3f SquareToBeckmann(const Point2f &sample, float alpha) {
  float phi = 2 * PI * sample.y();
  float theta = atan(sqrtf(-1 * alpha * alpha * log(1 - sample.x())));
  float z = cos(theta);
  float r = sqrtf(1.f - z * z);
  return Vector3f(r * cos(phi), r * sin(phi), z);
}

inline float SquareToBeckmannPdf(const Vector3f &m, float alpha) {
  float theta = acos(m.z());
  float D = 1 / (PI * alpha * alpha * cos(theta) * cos(theta) * cos(theta) * cos(theta))
      * exp(-tan(theta) * tan(theta) / (alpha * alpha));
  return m.z() > 0 ? D * cos(theta) : 0.f;
}

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLING_H_
