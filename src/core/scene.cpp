//
// Created by jzy99 on 2021/1/3.
//

#include <phoenix/core/scene.h>
#include<phoenix/core/camera.h>
#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

    inline void rtchit_to_interaction(const RTCRayHit &hit, Interaction &result) {
        result.isHit = hit.hit.geomID != RTC_INVALID_GEOMETRY_ID;
        if (!result.isHit)
            return;
        result.geoID = hit.hit.geomID;
        result.primID = hit.hit.primID;
        result.uv = Point2f(hit.hit.u, hit.hit.v);
        result.normal = Vector3f(hit.hit.Ng_x, hit.hit.Ng_y, hit.hit.Ng_z).normalized();
        result.tfar = hit.ray.tfar;
        result.geoFrame = Frame(result.normal);

    }

    void Scene::AddChild(shared_ptr<PhoenixObject> child) {
        switch (child->GetClassType()) {
            case PClassType::PCamera: {
                camera_ = std::dynamic_pointer_cast<Camera>(child);
                break;
            }
            case PClassType::PEmitter: {
                auto emitter = std::dynamic_pointer_cast<Emitter>(child);
                emitters_.push_back(emitter);
                light_dpdf_.Append(0);
                if (emitter->GetEmitterType() == Emitter::EmitterType::ENV) {
                    env_emtter_ = emitter;
                }
                spdlog::info("add light");
                break;
            }
            case PClassType::PIntegrator: {
                integrator_ = std::dynamic_pointer_cast<Integrator>(child);
                spdlog::info("add integrator");
                break;
            }
            case PClassType::PSampler: {
                sampler_ = std::dynamic_pointer_cast<Sampler>(child);
                spdlog::info("add sampler");
                break;
            }
            case PClassType::PScene: {
                break;
            }
            case PClassType::PShape: {
                auto shape = std::dynamic_pointer_cast<Shape>(child);
                if (shape->GetEmitter()) {
                    emitters_.push_back(shape->GetEmitter());
                    light_dpdf_.Append(shape->GetArea());
                }
                shapes_.push_back(shape);
                auto ids = shape->AddToEmbree(embree_);
                for (auto id:ids) {
                    shapes_dict_[id] = shape;
                }
                spdlog::info("add a shape");
                break;
            }
        }
    }

    Scene::Scene(const PropertyList &props) {
        light_dpdf_.Clear();

    }

    bool Scene::Intersect(const Ray &ray, Interaction &it) const {
        rtchit_to_interaction(embree_.CastRay(ray.orig_, ray.dir_, ray.mint_, ray.maxt_), it);
        if (!it.isHit)
            return false;
        auto shape = shapes_dict_.at(it.geoID);
        it.albedo = shape->GetTextureColor(it.geoID, it.primID, it.uv);
//        it.normal = shape->GetNormal(it.geoID, it.primID, it.uv, it.normal);
        it.point = ray.At(it.tfar);
        it.shape = shape;
        return true;

    }

    void Scene::Active() {
        embree_.EndAdd();
        light_dpdf_.normalize();
        spdlog::info("end add");
    }

    bool Scene::Intersect(const Ray &ray) const {
        auto res = embree_.CastRay(ray.orig_, ray.dir_, ray.mint_, ray.maxt_);
        return res.hit.primID != RTC_INVALID_GEOMETRY_ID;
    }

    shared_ptr<Emitter> Scene::GetRandomEmitter(shared_ptr<Sampler> sampler, float &pdf) {
        float random = sampler->Next1D();
        int index = random * 100000;
        index %= emitters_.size();
//        auto index = light_dpdf_.Sample(random, pdf);
        return emitters_.at(index);

    }

    shared_ptr<Emitter> Scene::GetEnvEmitter() const {
        return env_emtter_;
    }

    PHOENIX_REGISTER_CLASS(Scene, "scene");

PHOENIX_NAMESPACE_END


