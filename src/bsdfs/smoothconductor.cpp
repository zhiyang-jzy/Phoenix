//
// Created by jzy99 on 2021/4/25.
//

#include<phoenix/core/bsdf.h>
#include<phoenix/core/frame.h>
#include<phoenix/core/complexior.h>

PHOENIX_NAMESPACE_BEGIN

extern Color3f FresnelConductor(float cosI, const Color3f& eta, const Color3f& k);

class SmoothConductor : public BSDF {
 public:
  SmoothConductor(const PropertyList& props) {
    const std::string material = props.GetString("cior", "Au");

    ior_ = CiorLookup(material);
  }

  Color3f Eval(const BSDFQueryRecord &,Color3f albedo) const override{
    /* Discrete BRDFs always evaluate to zero in Nori */
    return Color3f(0.0f);
  }

   float Pdf(const BSDFQueryRecord &) const {
    /* Discrete BRDFs always evaluate to zero in Nori */
    return 0.0f;
  }

  Color3f Sample(BSDFQueryRecord &bRec, const Point2f &,Color3f albedo) const override {
    if (Frame::CosTheta(bRec.wi) <= 0)
      return Color3f(0.0f);

    // Reflection in local coordinates
    bRec.wo = Vector3f(
        -bRec.wi.x(),
        -bRec.wi.y(),
        bRec.wi.z()
    );

    /* Relative index of refraction: no change */
    bRec.eta = 1.0f;

    Color3f Fr = FresnelConductor(Frame::CosTheta(bRec.wo), ior_.eta, ior_.k);


    return Fr / Frame::CosTheta(bRec.wi);
  }

 private:
  ComplexIor ior_;
};

PHOENIX_REGISTER_CLASS(SmoothConductor,"smoothconductor");


PHOENIX_NAMESPACE_END