//
// Created by 焦智洋 on 2021/4/21.
//

#include<phoenix/core/bsdf.h>
#include<phoenix/core/frame.h>
#include<phoenix/core/sampling.h>

PHOENIX_NAMESPACE_BEGIN

    class Mircrofacet : public BSDF {
    private:
        float alpha_, intIOR_, extIOR_, ks_;
        Color3f kd_;
    public:
        Mircrofacet(const PropertyList &props) {
            alpha_ = props.GetFloat("alpha", 0.1f);
            intIOR_ = props.GetFloat("intIOR", 1.5046f);
            extIOR_ = props.GetFloat("extIOR", 1.00277f);
            kd_ = props.GetColor("kd", Color3f(0.5));
            ks_ = 1 - kd_.maxCoeff();

        }

        float EvalBeckmann(const Normal3f &m) const {
            float tmp = Frame::TanTheta(m) / alpha_,
                    ct = Frame::CosTheta(m), ct2 = ct * ct;

            return std::exp(-tmp * tmp) / (PI * alpha_ * alpha_ * ct2 * ct2);
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

        Color3f Eval(const BSDFQueryRecord &bRec, Color3f albedo) const override {
            Vector3f wh = (bRec.wi + bRec.wo).normalized();
            float D = EvalBeckmann(wh);
            float F = Fresnel((wh.dot(bRec.wo)), extIOR_, intIOR_);
            float G = (SmithBeckmannG1(bRec.wo, wh) * SmithBeckmannG1(bRec.wi, wh));

            return kd_ * INV_PI + ks_ * (D * F * G)
                                  / (4.f * Frame::CosTheta(bRec.wo) * Frame::CosTheta(bRec.wi));
        }

        [[nodiscard]] float Pdf(const BSDFQueryRecord &bRec) const override {
            Vector3f wh = (bRec.wi + bRec.wo).normalized();
            if (Frame::CosTheta(bRec.wo) <= 0)
                return 0.0f;

            float D = EvalBeckmann(wh);
            float Jh = 1.f / (4.f * abs(wh.dot(bRec.wo)));

            return ks_ * D * Frame::CosTheta(wh) * Jh + (1 - ks_) * Frame::CosTheta(bRec.wo) * INV_PI;
        }

        Color3f Sample(BSDFQueryRecord &bRec, const Point2f &_sample, Color3f albedo) const override {
            if (Frame::CosTheta(bRec.wi) <= 0)
                return Color3f(0.0f);

            if (_sample.x() < ks_) {
                Point2f sample(_sample.x() / ks_, _sample.y());
                Vector3f wh = SquareToBeckmann(sample, alpha_);
                bRec.wo = ((2.f * wh.dot(bRec.wi) * wh) - bRec.wi).normalized();
            } else {
                Point2f sample((_sample.x() - ks_) / (1 - ks_), _sample.y());
                bRec.wo = SquareToCosineHemisphere(sample);
            }

            if (Frame::CosTheta(bRec.wo) <= 0)
                return Color3f(0.0f);

            return Eval(bRec, albedo) / Pdf(bRec) * Frame::CosTheta(bRec.wo);
        }

    };

    PHOENIX_REGISTER_CLASS(Mircrofacet,"microfacet");

PHOENIX_NAMESPACE_END