#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/camera.h>
#include<phoenix/core/film.h>


PHOENIX_NAMESPACE_BEGIN

class PerspectiveCamera: public Camera{
public:
    PerspectiveCamera(const PropertyList& prop);
    void Active()override;
    float GenerateRay(const CameraSample& sample,Ray& ray) const override;
    void AddChild(shared_ptr<PhoenixObject> child) override;


 protected:

  shared_ptr<Film> film_;

  Point3f look_from_,look_at_;
  Vector3f look_up_;


  Transform screen_to_raster_,raster_to_screen_;
  Transform camera_to_screen_,screen_to_camera_;
  Transform raster_to_camera_;
  Transform camera_to_world_;
  float lens_radius_,focal_length_,fov_;



};



PHOENIX_NAMESPACE_END