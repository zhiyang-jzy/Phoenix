

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class PathmisIntegrator : public Integrator {
 public:
  PathmisIntegrator(const PropertyList &props) {}
  Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
    return {.0f};
  }

  Color3f SampleLight(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray, int depth = 0) {
    Interaction its;
    if (!scene->Intersect(ray, its)) {
      return Color3f(0.0f);
    }

    if (its.shape->GetBSDF()->IsDiffuse()) {
      if (sampler->Next1D() > 0.95f)
        return Color3f(.0f);
      else {
        BSDFQueryRecord bsdfQ(its.geoFrame.ToLocal(-ray.dir_));
        Color3f albedo = its.shape->GetBSDF()->Sample(bsdfQ, sampler->Next2D());
        return 1.057 * albedo * SampleLight(scene, sampler, Ray(its.point, its.geoFrame.ToWorld(bsdfQ.wo)));
      }
    }

    float emit_pdf;
    auto emit = scene->GetRandomEmitter(sampler, emit_pdf);
    EmitterQueryRecord eqr(its.point);
    Color3f Le = emit->Sample(eqr, sampler->Next2D());
    Vector3f distace_vec = eqr.wi.normalized();
    Color3f directColor = Color3f(.0f);

    float object_normal = abs(its.normal.dot(distace_vec));

    if (its.shape->IsEmitter()) {
      if (depth == 0) {
        return its.shape->GetEmitter()->Sample(eqr, sampler->Next2D());
      } else {
        return Color3f(.0f);
      }
    }

    BSDFQueryRecord bsdfQ = BSDFQueryRecord(its.geoFrame.ToLocal(ray.dir_), its.geoFrame.ToLocal(distace_vec));
    Color3f bsdf = its.shape->GetBSDF()->Eval(bsdfQ);

    float lightPdf = emit_pdf * emit->Pdf(eqr);

    float modifiedLightPdf = lightPdf;
    float distance = eqr.wi.dot(eqr.wi);
    modifiedLightPdf *= distance;

    float bsdfPdf = its.shape->GetBSDF()->Pdf(bsdfQ);
    float weight = modifiedLightPdf / (modifiedLightPdf + bsdfPdf);

    Color3f albedo = its.shape->GetBSDF()->Sample(bsdfQ, sampler->Next2D());


    Interaction temp_its;
    if (!scene->Intersect(eqr.shadowRay, temp_its) || its.shape->GetEmitter() != emit) {
      directColor = object_normal * Le * bsdf * 1.f / emit_pdf;
    }

    if (sampler->Next1D() < 0.95f && depth < 10)
      return weight * 1.f / 0.95 * (directColor
          + albedo * SampleLight(scene, sampler, Ray(its.point, its.geoFrame.ToWorld((bsdfQ.wo))), depth + 1));
    else
      return Color3f(0.f);

  }

  Color3f SampleBSDF(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) {

  }

};

PHOENIX_REGISTER_CLASS(PathmisIntegrator, "pathmis");

PHOENIX_NAMESPACE_END