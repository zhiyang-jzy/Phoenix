//
// Created by jzy99 on 2021/1/25.
//

#include<phoenix/core/bsdf.h>
#include<phoenix/core/sampling.h>
#include<phoenix/core/texture.h>

PHOENIX_NAMESPACE_BEGIN

class DiffuseBSDF : public BSDF {
 private:
  Color3f albedo_;
  bool cover_texture_;

 public:
  explicit DiffuseBSDF(const PropertyList &props) {
    albedo_ = props.GetColor("albedo", Color3f(0.5f));
    cover_texture_ = props.HasVal("albedo");
  }
  [[nodiscard]] Color3f Eval(const BSDFQueryRecord &rec) const override {
    if (rec.wi.z() <= 0 || rec.wo.z() <= 0)
      return Color3f(0.0f);

    return albedo_ * INV_PI;
  }

  Color3f GetTextureColor(unsigned int geoid,unsigned int primid,Point2f uv)const override{
    return albedo_;
  }

  float Pdf(const BSDFQueryRecord &rec) const {
    if (rec.wo.z() <= 0 || rec.wi.z() <= 0)
      return .0f;
    return INV_PI * rec.wo.normalized().z();
  }

  Color3f Sample(BSDFQueryRecord &rec, const Point2f &sample) const override {
    if (rec.wi.z() <= 0)
      return Color3f(0.f);
    rec.wo = SquareToCosineHemisphere(sample);
    rec.eta = 1.0f;

    return albedo_;
  }

  virtual Color3f Eval(const BSDFQueryRecord &rec, Color3f albedo) const override {
    if (rec.wi.z() <= 0 || rec.wo.z() <= 0)
      return Color3f(0.0f);

    if (cover_texture_)
      return albedo_ * INV_PI;
    return albedo * INV_PI;
  }

  Color3f Sample(BSDFQueryRecord &rec, const Point2f &sample, Color3f albedo) const override {
    if (rec.wi.z() <= 0)
      return Color3f(0.f);
    rec.wo = SquareToCosineHemisphere(sample);
    rec.eta = 1.0f;

    if (cover_texture_)
      return albedo_ ;

    return albedo;
  }

  bool IsDiffuse() const override {
    return true;
  }

};

PHOENIX_REGISTER_CLASS(DiffuseBSDF, "diffuse");

PHOENIX_NAMESPACE_END