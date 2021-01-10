//
// Created by jzy99 on 2021/1/3.
//

#include <phoenix/core/scene.h>
#include<spdlog/spdlog.h>
#include<phoenix/core/camera.h>
#include<phoenix/core/integrator.h>
#include<phoenix/core/sampler.h>

PHOENIX_NAMESPACE_BEGIN

    void Scene::addChild(const shared_ptr<PhoenixObject> child) {
        switch (child->getClassType()) {
            case PClassType::PCamera:{
                m_camera =  std::dynamic_pointer_cast<Camera>(child);
                break;
            }
                break;
            case PClassType::PEmitter:
                break;
            case PClassType::PIntegrator:{
                m_integrator = std::dynamic_pointer_cast<Integrator>(child);
                break;
            }
            case PClassType::Psampler:{
                m_sampler = std::dynamic_pointer_cast<Sampler>(child);
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


PHOENIX_NAMESPACE_END


