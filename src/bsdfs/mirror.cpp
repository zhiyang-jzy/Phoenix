//
// Created by jzy99 on 2021/1/25.
//
#include<phoenix/core/bsdf.h>
#include<phoenix/core/sampling.h>

PHOENIX_NAMESPACE_BEGIN

class MirrorBSDF : public BSDF {
 public:

  MirrorBSDF(const PropertyList &prop) {}


  Color3f Eval(const BSDFQueryRecord &rec, Color3f albedo) const override {
    return Color3f(0.0f);
  }

  float Pdf(const BSDFQueryRecord &) const override {
    return 0.0f;
  }

  Color3f Sample(BSDFQueryRecord &rec, const Point2f &sample, Color3f albedo) const override {
    if (rec.wi.z() <= 0)
      return Color3f(0.0f);

    // Reflection in local coordinates
    rec.wo = Vector3f(
        -rec.wi.x(),
        -rec.wi.y(),
        rec.wi.z()
    );

    /* Relative index of refraction: no change */
    rec.eta = 1.0f;

    return Color3f(1.0f);
  }

};

PHOENIX_REGISTER_CLASS(MirrorBSDF, "mirror");

PHOENIX_NAMESPACE_END