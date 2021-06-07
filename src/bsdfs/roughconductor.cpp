//
// Created by jzy99 on 2021/4/24.
//


#include<phoenix/core/bsdf.h>
#include<phoenix/core/complexior.h>
#include<phoenix/core/frame.h>
#include<phoenix/core/sampling.h>
PHOENIX_NAMESPACE_BEGIN

Color3f FresnelConductor(float cosI, const Color3f &eta, const Color3f &k) {
  Color3f tmp = (eta * eta + k * k) * cosI * cosI;
  Color3f Rparl2 = (tmp - (2.f * eta * cosI) + 1) / (tmp + (2.f * eta * cosI) + 1);
  Color3f tmp_f = eta * eta + k * k;
  Color3f Rperp2 =
      (tmp_f - (2.f * eta * cosI) + cosI * cosI) /
          (tmp_f + (2.f * eta * cosI) + cosI * cosI);
  return (Rparl2 + Rperp2) / 2.f;
}

class RoughConductor : public BSDF {
 public:
  RoughConductor(const PropertyList &propList) {
    /* RMS surface roughness */
    alpha_ = propList.GetFloat("alpha", 0.1f);
    const std::string &cior = propList.GetString("cior");
    ior_ = CiorLookup(cior);
  }

  float EvalBeckmann(const Normal3f &m) const {
    float tmp = Frame::TanTheta(m) / alpha_,
        ct = Frame::CosTheta(m), ct2 = ct * ct;

    return std::exp(-tmp * tmp) / (PI * alpha_ * alpha_ * ct2 * ct2);
  }

  float EvalGGX(const Normal3f &m) const {
    float ct = Frame::CosTheta(m), ct2 = ct * ct, a2 = alpha_ * alpha_;
    float tmp = ct2 * (a2 - 1) + 1;
    return a2 / (PI * tmp * tmp);
  }

  float GGXG1(const Vector3f &v, const Normal3f &m) const {
    float tan_theta = Frame::TanTheta(v);
  }

  float SmithBeckmannG1(const Vector3f &v, const Normal3f &m) const {
    float tanTheta = Frame::TanTheta(v);
    if (tanTheta == 0.0f)
      return 1.0f;

    if (m.dot(v) * Frame::CosTheta(v) <= 0)
      return 0.0f;

    float a = 1.0f / (alpha_ * tanTheta);
    if (a >= 1.6f)
      return 1.0f;
    float a2 = a * a;

    return (3.535f * a + 2.181f * a2)
        / (1.0f + 2.276f * a + 2.577f * a2);
  }

  /// Evaluate the BRDF for the given pair of directions
  /// Always assume that BSDFQueryRecord has directions in the local frame
  virtual Color3f Eval(const BSDFQueryRecord &bRec, Color3f albedo) const override {

    if (Frame::CosTheta(bRec.wi) <= 0 || Frame::CosTheta(bRec.wo) <= 0)
      return Color3f(0.0f);

    Normal3f w_h = (bRec.wi + bRec.wo).normalized();
    float D = EvalBeckmann(w_h);
    Color3f F = FresnelConductor(w_h.dot(bRec.wi), ior_.eta, ior_.k);
    float G = SmithBeckmannG1(bRec.wi, w_h) * SmithBeckmannG1(bRec.wo, w_h);

    return F * D * G / (4 * (Frame::CosTheta(bRec.wi) * Frame::CosTheta(bRec.wo)));
  }

  /// Evaluate the sampling density of \ref sample() wrt. solid angles
  float Pdf(const BSDFQueryRecord &bRec) const override {

    if (Frame::CosTheta(bRec.wo) <= 0.0f || Frame::CosTheta(bRec.wi) <= 0.0f) return 0.0f;
    Normal3f w_h = (bRec.wi + bRec.wo).normalized();
    float jacobian = 0.25f / (w_h.dot(bRec.wo));
    return SquareToBeckmannPdf(w_h, alpha_) * jacobian;
  }

  /// Sample the BRDF
  Color3f Sample(BSDFQueryRecord &bRec, const Point2f &_sample, Color3f albedo) const override {

    if (Frame::CosTheta(bRec.wi) <= 0)
      return Color3f(0.0f);

    float pdf;
    Normal3f w_h = SquareToBeckmann(_sample, alpha_);
    pdf = SquareToBeckmannPdf(w_h, alpha_);
    if (pdf == 0.0f) return 0.0f;
    bRec.wo = 2.0f * w_h.dot(bRec.wi) * w_h - bRec.wi;
    float jacobian = 0.25f / fabsf(w_h.dot(bRec.wo));
    pdf = pdf * jacobian;
    return Eval(bRec, albedo) / pdf;
  }

 private:
  float alpha_;
  ComplexIor ior_;
};

PHOENIX_REGISTER_CLASS(RoughConductor, "roughconductor");

PHOENIX_NAMESPACE_END

