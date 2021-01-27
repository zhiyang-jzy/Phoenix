//
// Created by jzy99 on 2021/1/26.
//

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class PathemsIntegrator : public Integrator {
 private:
  float russian_;
 public:
  explicit PathemsIntegrator(const PropertyList &props) {
    russian_ = props.GetFloat("russian", 0.95f);
  }
  [[nodiscard]] Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {

    Interaction its;
    Color3f res(0.0f);

    if (!scene->Intersect(ray, its))
      return Color3f(0.0f);


    if (its.shape->IsEmitter()) {
      EmitterQueryRecord iRec2(ray.orig_, its.point, its.normal);
      res += its.shape->GetEmitter()->Eval(iRec2);
      return res/russian_;
    }

    BSDFQueryRecord rec(its.geoFrame.ToLocal(-ray.dir_).normalized());

    Color3f bsdfvalue = its.shape->GetBSDF()->Sample(rec, sampler->Next2D());

    Ray light_ray(its.point, its.geoFrame.ToWorld(rec.wo));
    float cos0 = std::abs(its.geoFrame.n.normalized().dot(rec.wo.normalized()));

    if (sampler->Next1D() <= russian_)
      return res + Li(scene, sampler, light_ray) * bsdfvalue / russian_;

    return res;

  }

};

PHOENIX_REGISTER_CLASS(PathemsIntegrator, "pathems");

PHOENIX_NAMESPACE_END







