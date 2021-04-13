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

    if (!scene->Intersect(ray, its))
      return Color3f(.0f);

    if (its.shape->IsEmitter()) {
      EmitterQueryRecord iRec2(ray.orig_, its.point, its.normal);
      return its.shape->GetEmitter()->Eval(iRec2);
    }

    auto index = sampler->Next1DUInt() % scene->GetEmiitersCount();
    auto emitter = scene->GetEmitters()[index];

    Color3f L_dir(0.0f);

    EmitterQueryRecord ldir_que(its.point);
    auto emitter_color = emitter->Sample(ldir_que, sampler->Next2D());
    float cos_light = std::abs(ray.dir_.normalized().dot(its.normal.normalized()));

    if (!scene->Intersect(ldir_que.shadowRay))
      L_dir = emitter_color * scene->GetEmiitersCount()*cos_light/ldir_que.pdf;

    Color3f L_indir(0.0f);

    if (sampler->Next1D() <= russian_) {
      auto bsdf = its.shape->GetBSDF();
      auto bsdf_que = BSDFQueryRecord(its.geoFrame.ToLocal(-ray.dir_).normalized());
      auto bsdf_value = bsdf->Sample(bsdf_que, sampler->Next2D());

      Ray light_ray(its.point, its.geoFrame.ToWorld(bsdf_que.wo));
      float cos0 = std::abs(its.normal.normalized().dot(its.geoFrame.ToWorld(bsdf_que.wo).normalized()));
      L_indir = Li(scene, sampler, light_ray) * bsdf_value / russian_;
    }
    return L_indir + L_dir;
  }

};

PHOENIX_REGISTER_CLASS(PathemsIntegrator, "lightpath");

PHOENIX_NAMESPACE_END







