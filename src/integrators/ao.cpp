//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/integrator.h>
#include<phoenix/core/sampling.h>
#include<iostream>

PHOENIX_NAMESPACE_BEGIN

class AoIntegrator : public Integrator {
 public:
  AoIntegrator(const PropertyList& props) {

    /* No parameters this time */
  }

  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{


    Interaction its;
    if (!scene->Intersect(ray, its)) {
      return Color3f(0.f);
    }

    Color3f returnColor = Color3f(0.f);
    Point3f sample;
    Point3f samplePoint;
    Ray shadowRay;
    float cosine;
    sample = SquareToUniformHemisphere(sampler->Next2D());
    samplePoint = its.geoFrame.ToWorld(sample);
    shadowRay = Ray(its.point, samplePoint);
    Interaction shadow;

    if (!scene->Intersect(shadowRay,shadow)) {
      its.geoFrame.n.normalize();
      samplePoint.normalize();
      cosine = samplePoint.dot(its.geoFrame.n);
      returnColor = Color3f((cosine / PI)) / SquareToUniformHemispherePdf(sample);
    }

    return returnColor;

  }

};


PHOENIX_REGISTER_CLASS(AoIntegrator,"ao");

PHOENIX_NAMESPACE_END

