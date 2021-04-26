//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class AlbedoIntegrator: public Integrator{
 public:
  AlbedoIntegrator(const PropertyList& props){

  }

  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{
    Interaction it;
    if(!scene->Intersect(ray,it))
      return {0.0f};
    return it.shape->GetBSDF()->GetAlbedo(it.albedo);
  }
};

PHOENIX_REGISTER_CLASS(AlbedoIntegrator,"albedo");
PHOENIX_NAMESPACE_END