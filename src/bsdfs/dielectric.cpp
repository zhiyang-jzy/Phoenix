

#include<phoenix/core/bsdf.h>
#include<phoenix/core/common.h>

PHOENIX_NAMESPACE_BEGIN

class Dielectric : public BSDF {
 private:
  float intIOR_, extIOR_;

 public:



  explicit Dielectric(const PropertyList &props) {
    intIOR_ = props.GetFloat("intIOR", 1.5046f);
    extIOR_ = props.GetFloat("extIOR", 1.000277f);
  }



  Color3f Eval(const BSDFQueryRecord &rec, Color3f albedo) const override {
    Vector3f reflec = GetReflection(rec);
    Vector3f refrac = GetRefraction(rec);

    bool isRefl = rec.wo.isApprox(reflec, 1e-3);
    bool isRefr = rec.wo.isApprox(reflec, 1e-3);

    float n1 = extIOR_;
    float n2 = intIOR_;

    float cosT = rec.wi.z();

    if (cosT < 0) {
      n1 = intIOR_;
      n2 = extIOR_;
    }

    float snell = n1 / n2;

    if (isRefl)
      return Color3f(1.f);
    else if (isRefr)
      return snell * snell * Color3f(1.f);
    else
      return Color3f(0.0f);
  }

  float Pdf(const BSDFQueryRecord &bRec) const override {

    Vector3f n(0, 0, 1);
    float n1 = extIOR_;
    float n2 = intIOR_;

    float cosT = bRec.wi.z();

    if (cosT < 0) {
      n1 = intIOR_;
      n2 = extIOR_;
      n = -n;
      cosT = -cosT;
    }

    float F = Fresnel(cosT, n1, n2);

    Vector3f refrac = GetRefraction(bRec);
    Vector3f reflec = GetReflection(bRec);

    float prob = 0;

    if (bRec.wo.isApprox(refrac, EPSILON) || (bRec.wo.isApprox(reflec, EPSILON)))
      prob = 1.f;
    else
      prob = 0.f;

    return prob;
  }


  Color3f Sample(BSDFQueryRecord &bRec, const Point2f &sample, Color3f albedo) const override {
    Vector3f n(0, 0, 1);
    float n1 = extIOR_;
    float n2 = intIOR_;

    float cosT = bRec.wi.z();

    if (cosT < 0) {
      n1 = intIOR_;
      n2 = extIOR_;
      n = -n;
      cosT = -cosT;
    }

    float F = Fresnel(cosT, n1, n2);
    float snell = n1 / n2;

    float cons = sqrt(1.0f - (snell * snell) * (1.0f - cosT * cosT));

    bool isTReflec = cons > 1;

    if (sample.x() < F || isTReflec) {
      bRec.wo = GetReflection(bRec);

      return Color3f(1.f);

    } else {
      bRec.wo = GetRefraction(bRec);

      return snell * snell * Color3f(1.f);

    }
  }

 protected:

  [[nodiscard]] Vector3f GetOutgoingVector(const BSDFQueryRecord &bRec, bool returnRefract) const {
    Vector3f n(0, 0, 1);
    float n1 = extIOR_;
    float n2 = intIOR_;

    float cosT = bRec.wi.z();

    if (cosT < 0) {
      n1 = intIOR_;
      n2 = extIOR_;
      n = -n;
      cosT = -cosT;
    }

    float snell = n1 / n2;

    Vector3f part1 = -snell * (bRec.wi - cosT * n);
    float cons = sqrt(1.0f - (snell * snell) * (1.0f - cosT * cosT));
    Vector3f part2 = n * cons;

    Vector3f refraction = part1 - part2;
    refraction = refraction.normalized();

    Vector3f reflection = Vector3f(-bRec.wi.x(), -bRec.wi.y(), bRec.wi.z());

    if (returnRefract)
      return refraction;
    else
      return reflection;
  }

  [[nodiscard]] Vector3f GetReflection(const BSDFQueryRecord &bRec) const {
    return GetOutgoingVector(bRec, false);
  }

  [[nodiscard]] Vector3f GetRefraction(const BSDFQueryRecord &bRec) const {
    return GetOutgoingVector(bRec, true);
  }
};

PHOENIX_REGISTER_CLASS(Dielectric, "dielectric");

PHOENIX_NAMESPACE_END
