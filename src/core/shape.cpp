//
// Created by jzy99 on 2021/1/25.
//

#include<phoenix/core/shape.h>
#include<spdlog/spdlog.h>

PHOENIX_NAMESPACE_BEGIN


void Shape::AddChild(shared_ptr<PhoenixObject> child) {
  switch (child->GetClassType()) {
    case PhoenixObject::PClassType::PEmitter:{
      emitter_ = std::dynamic_pointer_cast<Emitter>(child);
      emitter_->SetShape(shared_ptr<Shape>(this));
      spdlog::info("add emitter");
      break;
    }
    case PhoenixObject::PClassType::PBSDF:{
      bsdf_ = std::dynamic_pointer_cast<BSDF>(child);
      spdlog::info("add bsdf");
      break;
    }
  }
}
void Shape::Active() {
  if(!bsdf_)
  {
    bsdf_ = std::dynamic_pointer_cast<BSDF>(PhoenixObjectFactory::CreateInstance("diffuse",PropertyList()));
  }
}

PHOENIX_NAMESPACE_END
