//
// Created by jzy99 on 2021/1/23.
//

#include<phoenix/core/camera.h>
#include<spdlog/spdlog.h>

PHOENIX_NAMESPACE_BEGIN


void Camera::AddChild(shared_ptr<PhoenixObject> child) {
  switch (child->GetClassType()) {
    case PClassType::PFilter: {
      filter_ = std::dynamic_pointer_cast<Filter>(child);
      spdlog::info("add filter");

      break;
    }
  }
}

PHOENIX_NAMESPACE_END

