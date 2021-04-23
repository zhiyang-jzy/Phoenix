//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_SCENE_H
#define PHOENIX_SCENE_H

#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>
#include<phoenix/core/shape.h>
#include<phoenix/core/dpdf.h>
#include "pembree.h"

PHOENIX_NAMESPACE_BEGIN

class Scene : public PhoenixObject {
 public:
  shared_ptr<Camera> camera_;
  vector<shared_ptr<Emitter>> emitters_;
  shared_ptr<Sampler> sampler_;
  shared_ptr<Integrator> integrator_;
  vector<shared_ptr<Shape> > shapes_;
  Pembree embree_;
  shared_ptr<Emitter> env_emtter_;
  unordered_map<unsigned int, shared_ptr<Shape>> shapes_dict_;
  DiscretePDF light_dpdf_;


 public:
  [[nodiscard]] PClassType GetClassType() const override { return PClassType::PScene; }
  [[nodiscard]] string ToString() const override { return "scene"; }

  explicit Scene(const PropertyList &props);

  [[nodiscard]] unsigned int GetEmiitersCount()const {return emitters_.size();}
  [[nodiscard]] const vector<shared_ptr<Emitter>>& GetEmitters()const{return emitters_;}
  bool Intersect(const Ray &ray, Interaction &it) const;
  [[nodiscard]] bool Intersect(const Ray &ray)const;
  shared_ptr<Emitter> GetEnvEmitter()const;
  void AddChild(shared_ptr<PhoenixObject> child) override;
  void Active() override;
  shared_ptr<Emitter> GetRandomEmitter(shared_ptr<Sampler> sampler,float & pdf);

};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_SCENE_H
