//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/integrator.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/interaction.h>
#include<phoenix/core/ray.h>
#include<phoenix/core/common.h>

PHOENIX_NAMESPACE_BEGIN


class SimpleIntegrator: public Integrator{
 protected:
  Point3f light_pos_;
  Color3f energy_;
 public:
  SimpleIntegrator(const PropertyList& props){
    light_pos_  = props.GetPoint("position");
    energy_ = props.GetColor("energy");
  }

  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{
    Interaction its;
    if (!scene->Intersect(ray, its))
      return Color3f(0.0f);

    Interaction its2;

    Vector3f dir = (its.point - light_pos_).normalized();
    Ray ray2(light_pos_, dir);


    scene->Intersect(ray2, its2);

    float lighted = 0;
    if (its2.point.isApprox(its.point, 1e-3))
      lighted = 1;

    float cosin = (-dir).dot(its.normal);

    return 0.25f*INV_PI*INV_PI*lighted*energy_*fmax(cosin,0)/(light_pos_-its.point).squaredNorm();
  }

};

PHOENIX_REGISTER_CLASS(SimpleIntegrator,"simple");

PHOENIX_NAMESPACE_END