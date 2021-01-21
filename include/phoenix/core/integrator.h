//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_INTEGRATOR_H
#define PHOENIX_INTEGRATOR_H

#include<phoenix/core/object.h>
#include<phoenix/core/phoenix.h>
#include<phoenix/core/color.h>
#include<phoenix/core/scene.h>

PHOENIX_NAMESPACE_BEGIN

class Integrator : public PhoenixObject {
 public:

  [[nodiscard]] PhoenixObject::PClassType GetClassType()const override{return PhoenixObject::PClassType::PIntegrator;}

  [[nodiscard]] virtual Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray)const = 0;

  [[nodiscard]] string ToString()const override{return "";}



};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INTEGRATOR_H
