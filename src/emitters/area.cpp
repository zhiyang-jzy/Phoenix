//
// Created by jzy99 on 2021/1/24.
//

#include<phoenix/core/emitter.h>
#include<phoenix/core/color.h>

#include<phoenix/core/sampledata.h>
#include<phoenix/core/shape.h>

PHOENIX_NAMESPACE_BEGIN


    class AreaLight : public Emitter {

    public:
        Color3f radiance;
    public:

        [[nodiscard]] EmitterType GetEmitterType() const override {
            return EmitterType::AREA;
        }

        explicit AreaLight(const PropertyList &props) {
            radiance = props.GetColor("radiance");
        }

        Color3f Sample(EmitterQueryRecord &lRec, const Point2f &sample) const override {

            SampleData sRec = shape_->SampleSurface(sample);

            lRec.p = sRec.point;
            lRec.pdf = sRec.pdf;
            lRec.n = sRec.normal;
            lRec.wi = (lRec.p - lRec.ref).normalized();

            Ray shadowRay(lRec.ref, lRec.wi, EPSILON,
                          (lRec.p - lRec.ref).norm() - EPSILON);
            lRec.shadowRay = shadowRay;
            if (Pdf(lRec) > 0)
                return Eval(lRec) / Pdf(lRec);
            else
                return Color3f(0.f);
            return {0, 0, 0};

        }

        [[nodiscard]] Color3f Eval(const EmitterQueryRecord &lRec) const override {
            float cos_theta_i = lRec.n.dot(-lRec.wi);

            if ((cos_theta_i) >= 0) {
                return radiance;
            } else {
                return Color3f(0.f);
            }
        }

        Color3f GetColor() const override {
            return radiance;
        }

        [[nodiscard]] float Pdf(const EmitterQueryRecord &lRec) const override {
            SampleData sRec(lRec.ref, lRec.p);
            sRec.normal = lRec.n;

            float cosin = lRec.n.dot(-lRec.wi);
            if (cosin <= 0) {
                return 0;
            }
            return shape_->PdfSurface(sRec) * (lRec.p - lRec.ref).squaredNorm() / cosin;
        }
    };

    PHOENIX_REGISTER_CLASS(AreaLight, "area");

PHOENIX_NAMESPACE_END

