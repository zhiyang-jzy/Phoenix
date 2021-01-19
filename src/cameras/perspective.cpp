//
// Created by jzy99 on 2021/1/2.
//

#include<phoenix/cameras/perspective.h>
#include<phoenix/core/properlist.h>

PHOENIX_NAMESPACE_BEGIN



PerspectiveCamera::PerspectiveCamera(const PropertyList &prop) {

//  focal_length_ = prop.GetFloat("focal_length");
//  look_from_ = prop.GetPoint("look_from");
//  look_at_ = prop.GetPoint("look_at");
//  look_up_ = prop.GetVector("look_from");
    fov_ = prop.GetFloat("fov");
    camera_to_world_ = prop.GetTransform("toWorld");


}

float PerspectiveCamera::GenerateRay(const CameraSample &sample, Ray &ray) const {

  Point3f film = Point3f(sample.film.x(),sample.film.y(),0);
  Point3f camera = raster_to_camera_*film;



  return .1f;
}
void PerspectiveCamera::Active() {

}
void PerspectiveCamera::AddChild(shared_ptr<PhoenixObject> child) {
  switch (child->GetClassType()) {
    case PClassType::PFilm:
      film_ = std::dynamic_pointer_cast<Film>(child);
      break;

  }
}
PHOENIX_REGISTER_CLASS(PerspectiveCamera,"perspective");
PHOENIX_NAMESPACE_END