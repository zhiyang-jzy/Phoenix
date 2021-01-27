//
// Created by jzy99 on 2021/1/10.
//

#ifndef PHOENIX_BSDF_H
#define PHOENIX_BSDF_H

#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>
#include<phoenix/core/vector.h>
#include<phoenix/core/properlist.h>

PHOENIX_NAMESPACE_BEGIN

struct BSDFQueryRecord {
  /// Incident direction (in the local frame)
  Vector3f wi;

  /// Outgoing direction (in the local frame)
  Vector3f wo;

  /// Relative refractive index in the sampled direction
  float eta;

  /// Create a new record for sampling the BSDF
  BSDFQueryRecord(Vector3f wi)
      : wi(std::move(wi)), eta(1.f) {}

  /// Create a new record for querying the BSDF
  BSDFQueryRecord(Vector3f wi,
                  Vector3f wo)
      : wi(std::move(wi)), wo(std::move(wo)), eta(1.f) {}
};

class BSDF : public PhoenixObject {
 public:
  virtual Color3f Sample(BSDFQueryRecord &rec, const Point2f &sample) const = 0;

  virtual Color3f Eval(const BSDFQueryRecord &rec) const = 0;

  virtual float Pdf(const BSDFQueryRecord &rec) const = 0;

  PhoenixObject::PClassType GetClassType() const { return PhoenixObject::PClassType::PBSDF; }

  virtual bool IsDiffuse() const { return false; }

  string ToString()const override{return "";}
};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_BSDF_H
