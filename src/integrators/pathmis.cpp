

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class PathmisIntegrator : public Integrator {
 public:
  PathmisIntegrator(const PropertyList &props) {}
  Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
    return {.0f};
  }

  Color3f SampleLight(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) {
    Interaction its;
    if (!scene->Intersect(ray, its)) {
      return Color3f(0.0f);
    }

    if (its.shape->IsEmitter()) {
      EmitterQueryRecord iRec2(ray.orig_, its.point, its.normal);
      return its.shape->GetEmitter()->Eval(iRec2);
    }

    float light_pdf;
    auto emitter = scene->GetRandomEmitter(sampler,light_pdf);


    EmitterQueryRecord eqr(its.point);
    Color3f Le = emitter->Sample(eqr,sampler->Next2D());
    light_pdf*=eqr.pdf;

    BSDFQueryRecord bsdfQ();

    return {0.0f};

  }

  Color3f SampleBSDF(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) {

  }

};

PHOENIX_REGISTER_CLASS(PathmisIntegrator, "pathmis");

PHOENIX_NAMESPACE_END