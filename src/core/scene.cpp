//
// Created by jzy99 on 2021/1/3.
//

#include <phoenix/core/scene.h>
#include<spdlog/spdlog.h>
#include<phoenix/core/camera.h>
#include<phoenix/core/integrator.h>
#include<phoenix/core/sampler.h>
#include<phoenix/core/ray.h>
#include<phoenix/core/interaction.h>

PHOENIX_NAMESPACE_BEGIN

inline void rtchit_to_interaction(const RTCRayHit& hit,Interaction& result)
{
  result.isHit = !(hit.hit.geomID == RTC_INVALID_GEOMETRY_ID);
  if(!result.isHit)
    return ;
  result.geoID=hit.hit.geomID;
  result.primID=hit.hit.primID;
  result.uv = Point2f(hit.hit.u,hit.hit.v);
  result.normal = Vector3f(hit.hit.Ng_x,hit.hit.Ng_y,hit.hit.Ng_z);
  result.tfar = hit.ray.tfar;

}

void Scene::AddChild(shared_ptr<PhoenixObject> child) {
  switch (child->GetClassType()) {
    case PClassType::PCamera: {
      camera_ = std::dynamic_pointer_cast<Camera>(child);
      break;
    }
    case PClassType::PEmitter:break;
    case PClassType::PIntegrator: {
      integrator_ = std::dynamic_pointer_cast<Integrator>(child);
      break;
    }
    case PClassType::PSampler: {
      sampler_ = std::dynamic_pointer_cast<Sampler>(child);
      break;
    }
    case PClassType::PScene: {
      throw std::exception("waea");
      break;
    }
    case PClassType::PShape: {
      auto shape = std::dynamic_pointer_cast<Shape>(child);
      shapes_.push_back(shape);
      unsigned int id = shape->AddToEmbree(embree_);
      shapes_dict_[id] = shape;
      spdlog::info("add a shape");
      break;
    }
  }
}
Scene::Scene(const PropertyList &props) {

}
bool Scene::Intersect(const Ray &ray, Interaction &it) const {
  rtchit_to_interaction(embree_.CastRay(ray.orig_,ray.dir_),it);
  if(!it.isHit)
    return false;
  auto shape = shapes_dict_.at(it.primID);
  it.point = ray.At(it.tfar);
  it.shape = shape;
  return true;

}
void Scene::Active() {
  embree_.EndAdd();
  spdlog::info("end add");
}

PHOENIX_REGISTER_CLASS(Scene,"scene");

PHOENIX_NAMESPACE_END


