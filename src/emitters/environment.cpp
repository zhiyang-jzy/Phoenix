//
// Created by 焦智洋 on 2021/4/20.
//

#include<phoenix/core/emitter.h>
#include<phoenix/core/color.h>

#include<phoenix/core/sampledata.h>
#include<phoenix/core/shape.h>
#include<phoenix/core/envmap.h>
#include<filesystem>

PHOENIX_NAMESPACE_BEGIN

    class EnvironmentLight : public Emitter {
    private:
        Envmap envmap_;


    public:
        EmitterType GetEmitterType() const override {
            return EmitterType::ENV;
        }

        EnvironmentLight(const PropertyList &props) {
            std::filesystem::path path(props.GetString("filepath"));
            auto now_path = absolute(path);
            envmap_ = Envmap(now_path.string());
        }

        Color3f Eval(const EmitterQueryRecord &lRec) const override {
            return envmap_.Eval(lRec.wi);
        }

        Color3f Sample(EmitterQueryRecord &lRec, const Point2f &sample) const override {
            Color3f col = envmap_.Sample(lRec.wi, sample);
            lRec.shadowRay = Ray(lRec.ref, lRec.wi, EPSILON, 10000);
            return col;
        }

        float Pdf(const EmitterQueryRecord &lRec) const override {
            return envmap_.Pdf(lRec.wi);
        }
    };

    PHOENIX_REGISTER_CLASS(EnvironmentLight, "env");

PHOENIX_NAMESPACE_END