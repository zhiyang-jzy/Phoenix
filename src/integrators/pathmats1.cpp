//
// Created by jzy99 on 2021/1/25.
//



#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class PathmatsIntegrator1 : public Integrator {
 protected:
  float russian_;

 public:
  PathmatsIntegrator1(const PropertyList &props) {
    russian_ = props.GetFloat("russian", 0.95f);
  }

  Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
    Interaction isect;
    Ray trace_ray = ray;
    Color3f Li(0.0f), Beta(1.0f);
    uint32_t depth = 0;

    while (depth < 50) {
      if (!scene->Intersect(trace_ray, isect)) {
        if (scene->GetEnvEmitter() != nullptr) {
          EmitterQueryRecord eqr;
          eqr.wi = trace_ray.dir_;
          Li += Beta * scene->GetEnvEmitter()->Eval(eqr);
        } else if (depth == 0) {
          return Color3f(0.0f);
        }
        break;
      }
      if (isect.shape->IsEmitter()) {
        EmitterQueryRecord iRec2(ray.orig_, isect.point, isect.normal);
        Color3f Le = isect.shape->GetEmitter()->Eval(iRec2);
        Li += Le * Beta;
        break;
      }

      auto bsdf = isect.shape->GetBSDF();
      BSDFQueryRecord rec(isect.geoFrame.ToLocal(-ray.dir_).normalized());
      Beta *= bsdf->Eval(rec, isect.albedo);

      if (Beta.isZero()) {
        break;
      }
      if (sampler->Next1D() < 0.95) {
        Beta *= (1.0f / 0.95);
      } else {
        break;
      }
      trace_ray = Ray(isect.point, isect.geoFrame.ToWorld(rec.wo));
      depth++;
    }
    return Li;
  }

};

PHOENIX_REGISTER_CLASS(PathmatsIntegrator1, "pathnaive1");

PHOENIX_NAMESPACE_END