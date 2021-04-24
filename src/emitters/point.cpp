//
// Created by 焦智洋 on 2021/4/21.
//

#include<phoenix/core/emitter.h>

PHOENIX_NAMESPACE_BEGIN


    class PointLight : public Emitter {
    public:

        EmitterType GetEmitterType()const override{
            return EmitterType::POINT;
        }

        PointLight(const PropertyList &propList) {
            position_ = propList.GetPoint("position", Point3f(0,0,0));
            power_ = propList.GetColor("power", Color3f(10,10,10));
        }

        virtual Color3f Sample(EmitterQueryRecord &lRec, const Point2f &sample) const override {
            lRec.p = position_;
            lRec.wi = (position_ - lRec.ref).normalized();
            // shadowray term to check that sampled point and light source are mutually visible
            lRec.shadowRay = Ray(lRec.ref, lRec.wi, EPSILON, (lRec.p - lRec.ref).norm());

            return power_ / (4 * PI * (lRec.ref - lRec.p).squaredNorm());
        }

        virtual Color3f Eval(const EmitterQueryRecord &lRec) const override {
            return power_;
        }

        virtual float Pdf(const EmitterQueryRecord &lRec) const override {
            return 1;
        }


    private:
        Color3f power_;
        Point3f position_;
    };

    PHOENIX_REGISTER_CLASS(PointLight,"point");

PHOENIX_NAMESPACE_END