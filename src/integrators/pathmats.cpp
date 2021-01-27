//
// Created by jzy99 on 2021/1/25.
//



#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class PathmatsIntegrator : public Integrator {

 public:
  PathmatsIntegrator(const PropertyList &props) {
  }

  Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
    Interaction its;

    // If not visible return black
    if (!scene->Intersect(ray, its))
      return Color3f(0.0f);


    //Get Le
    Color3f Le = Color3f(0.0f);
    auto emi2 = its.shape->GetEmitter();
    if (emi2) {
      EmitterQueryRecord iRec2(ray.orig_, its.point, its.normal);
      Le = emi2->Eval(iRec2);
    }

    // No Ld

    //Get Li
    auto bsdf = its.shape->GetBSDF();
    Vector3f toCam = -ray.dir_.normalized();

    BSDFQueryRecord query(its.geoFrame.ToLocal(toCam)); //wi Camera, wo sampled ray
    //query.p = its.p;
    Color3f bsdfVal = bsdf->Sample(query, sampler->Next2D());

    //Check if intersect with emitter
    Ray lightRay(its.point, its.geoFrame.ToWorld(query.wo));

    if (sampler->Next1D() > 0.05)
      return Le + this->Li(scene, sampler, lightRay) * bsdfVal / (1.f - 0.05);
    else
      return Le;
  }
};

PHOENIX_REGISTER_CLASS(PathmatsIntegrator, "pathmats");

PHOENIX_NAMESPACE_END