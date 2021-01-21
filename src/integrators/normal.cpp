//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/integrator.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/scene.h>
#include <phoenix/core/interaction.h>

PHOENIX_NAMESPACE_BEGIN

class NormalIntegrator: public Integrator{
 public:
  NormalIntegrator(const PropertyList& props){

  }

  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{
    Interaction it;
    if(!scene->Intersect(ray,it))
      return {0.0f};

    Vector3f norm = it.normal;
    return Color3f(norm.array());

  }

};



PHOENIX_REGISTER_CLASS(NormalIntegrator,"normals");



PHOENIX_NAMESPACE_END