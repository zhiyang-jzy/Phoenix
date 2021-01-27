//
// Created by jzy99 on 2021/1/25.
//

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class TestligthIntegrator: public Integrator
{
 public:
  TestligthIntegrator(const PropertyList& prop)
  {
  }

  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{
    Interaction its;
    if (!scene->Intersect(ray, its))
      return Color3f(0.0f);

    if(its.shape->GetEmitter())
    {
      EmitterQueryRecord query_record(ray.orig_,its.point,its.normal);
      return its.shape->GetEmitter()->Eval(query_record);
//      return Color3f(1,1,1);
    }
    else{
      return Color3f(0.0f);
    }

  }

};

PHOENIX_REGISTER_CLASS(TestligthIntegrator,"testlight");

PHOENIX_NAMESPACE_END
