//
// Created by jzy99 on 2021/1/3.
//

#include <phoenix/core/scene.h>
#include<spdlog/spdlog.h>
#include<phoenix/core/camera.h>

PHOENIX_NAMESPACE_BEGIN

    void Scene::addChild(shared_ptr<PhoenixObject> child) {
        switch (child->getClassType()) {
            case PClassType::PCamera:{
                if(m_camera)
                    throw std::exception("already has ");
                m_camera =  std::static_pointer_cast<shared_ptr<Camera>>(child);
                break;
            }
                break;
            case PClassType::PEmitter:
                break;
            case PClassType::PIntegrator:{
                m_integrator = std::static_pointer_cast<shared_ptr<Integrator>>(child);
                break;
            }
            case PClassType::Psampler:{
                m_sampler = std::static_pointer_cast<shared_ptr<Sampler>>(child);
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


