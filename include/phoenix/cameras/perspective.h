#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/camera.h>
#include<phoenix/core/film.h>
#include<phoenix/core/transform.h>


PHOENIX_NAMESPACE_BEGIN

class PerspectiveCamera: public Camera{
public:
    PerspectiveCamera(const PropertyList& prop);
    void Active()override;
    float GenerateRay(const CameraSample& sample,Ray& ray) const override;

 private:

  shared_ptr<Film> film_;
  Transform camera_to_world_,sample_to_camera_;
  float near_clip_,far_clip_,fov_;
  Vector2f inv_output_size_;


};



PHOENIX_NAMESPACE_END