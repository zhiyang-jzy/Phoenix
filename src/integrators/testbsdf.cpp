//
// Created by jzy99 on 2021/1/25.
//

#include<phoenix/core/integrator.h>
PHOENIX_NAMESPACE_BEGIN


class BSDFTestIntegrator: public Integrator{
 private:
  Color3f albedo_;


 public:

  BSDFTestIntegrator(const PropertyList& props)
  {
    albedo_ = props.GetColor("albedo",Color3f(0.5f));
  }

  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{
    Interaction its;
    if (!scene->Intersect(ray, its) || scene->emitters_.size()==0)
      return Color3f(0.0f);

    if(scene->emitters_.size()==0)
      return Color3f(0.0f);

    Color3f result(0.0f);
    auto bsdf = its.shape->GetBSDF();

    unsigned int index = rand()%scene->emitters_.size();
    auto emi = scene->emitters_[index];
    EmitterQueryRecord iRec = EmitterQueryRecord(its.point);
    auto emi2 = its.shape->GetEmitter();
    if (emi2)
    {
      EmitterQueryRecord iRec2(ray.orig_, its.point, its.normal);
      result = result + emi2->Eval(iRec2);
    }

    Color3f Lo = scene->emitters_.size() * emi->Sample(iRec, sampler->Next2D()); //Instead of iterating over all emitter, mulply because MC

    Interaction its2;

    if (scene->Intersect(iRec.shadowRay, its2))
      Lo = Color3f(0);

    BSDFQueryRecord query = BSDFQueryRecord(its.geoFrame.ToLocal(iRec.wi),
                                            its.geoFrame.ToLocal(-ray.dir_));
    Color3f bsdfVal = bsdf->Eval(query);
    float cos0 = std::abs(its.geoFrame.n.dot(iRec.wi));
    result = result + Lo * bsdfVal * cos0;
    return result;
  }


};


PHOENIX_REGISTER_CLASS(BSDFTestIntegrator,"testbsdf");


PHOENIX_NAMESPACE_END