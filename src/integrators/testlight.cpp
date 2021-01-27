//
// Created by jzy99 on 2021/1/25.
//

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class TestbsdfIntegrator: public Integrator
{
 public:
  TestbsdfIntegrator(const PropertyList& prop)
  {
  }

  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{
    Interaction its;

    if (!scene->Intersect(ray, its))
      return Color3f(0.0f);



  }

};

PHOENIX_REGISTER_CLASS(TestbsdfIntegrator,"testbsdf");

PHOENIX_NAMESPACE_END
