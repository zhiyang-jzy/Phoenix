//
// Created by jzy99 on 2021/1/25.
//

#include<phoenix/core/bsdf.h>
#include<phoenix/core/sampling.h>

PHOENIX_NAMESPACE_BEGIN

class DiffuseBSDF : public BSDF {
 private:
  Color3f albedo_;
 public:
  explicit DiffuseBSDF(const PropertyList &props) {
    albedo_ = props.GetColor("albedo", Color3f(0.5f));
  }
  [[nodiscard]] Color3f Eval(const BSDFQueryRecord &rec) const override {
    if (rec.wi.z() <= 0 || rec.wo.z() <= 0)
      return Color3f(0.0f);

    return albedo_ * INV_PI;
  }

  float Pdf(const BSDFQueryRecord &rec) const {
    if (rec.wo.z() <= 0 || rec.wi.z() <= 0)
      return .0f;
    return INV_PI * rec.wo.normalized().z();
  }

  Color3f Sample(BSDFQueryRecord &rec, const Point2f &sample)const override {
    if (rec.wi.z() <= 0)
      return Color3f(0.f);
    rec.wo = SquareToCosineHemisphere(sample);
    rec.eta = 1.0f;

    return albedo_;
  }

  bool IsDiffuse() const override {
    return true;
  }

};

PHOENIX_REGISTER_CLASS(DiffuseBSDF, "diffuse");

PHOENIX_NAMESPACE_END