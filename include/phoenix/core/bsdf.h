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
  Vector3f wi;
  Vector3f wo;
  float eta;
  Point2f uv;

  /// Create a new record for sampling the BSDF
  BSDFQueryRecord(Vector3f wi)
      : wi(std::move(wi)), eta(1.f) {}

  BSDFQueryRecord(Vector3f wi,
                  Vector3f wo)
      : wi(std::move(wi)), wo(std::move(wo)), eta(1.f) {}
};

class BSDF : public PhoenixObject {
 public:
  [[nodiscard]] virtual Color3f GetAlbedo(Color3f texcolor)const {return texcolor;}
  virtual Color3f Sample(BSDFQueryRecord &rec, const Point2f &sample, Color3f albedo) const {
    return Color3f(0.3,
                   0.4,
                   0.5);
  }


  virtual Color3f Eval(const BSDFQueryRecord &rec, Color3f albedo) const { return Color3f(0.3, 0.4, 0.5); }

  virtual float Pdf(const BSDFQueryRecord &rec) const =0;

  virtual Color3f GetTextureColor(unsigned int geoid, unsigned int primid, Point2f uv) const { return {0.3, 0.4, 0.5}; }

  PhoenixObject::PClassType GetClassType() const override{ return PhoenixObject::PClassType::PBSDF; }

  virtual bool IsDiffuse() const { return false; }

  string ToString() const override { return ""; }
};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_BSDF_H
