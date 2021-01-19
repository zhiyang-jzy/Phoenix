//
// Created by jzy99 on 2021/1/3.
//

#include <phoenix/core/scene.h>
#include<spdlog/spdlog.h>
#include<phoenix/core/camera.h>
#include<phoenix/core/integrator.h>
#include<phoenix/core/sampler.h>

PHOENIX_NAMESPACE_BEGIN

    void Scene::AddChild(const shared_ptr<PhoenixObject> child) {
        switch (child->GetClassType()) {
            case PClassType::PCamera:{
                camera_ =  std::dynamic_pointer_cast<Camera>(child);
                break;
            }
                break;
            case PClassType::PEmitter:
                break;
            case PClassType::PIntegrator:{
                integrator_ = std::dynamic_pointer_cast<Integrator>(child);
                break;
            }
            case PClassType::PSampler:{
                sampler_ = std::dynamic_pointer_cast<Sampler>(child);
                break;
            }
            case PClassType::PScene:{
                throw std::exception("waea");
            }
            case PClassType::PShape:
            {

            }
        }
    }
Scene::Scene(const PropertyList &props) {

}

PHOENIX_REGISTER_CLASS(Scene,"scene");

PHOENIX_NAMESPACE_END


