//
// Created by jzy99 on 2021/1/2.
//

#include<phoenix/cameras/perspective.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/common.h>
#include<phoenix/core/ray.h>

PHOENIX_NAMESPACE_BEGIN

PerspectiveCamera::PerspectiveCamera(const PropertyList &prop) {
  fov_ = prop.GetFloat("fov");
  camera_to_world_ = prop.GetTransform("toWorld");
  output_size_.x() = prop.GetInteger("width", 1280);
  output_size_.y() = prop.GetInteger("height", 720);
  inv_output_size_ = output_size_.cast<float>().cwiseInverse();

  near_clip_ = prop.GetFloat("nearClip", 1e-4f);
  far_clip_ = prop.GetFloat("farClip", 1e4f);
  filter_ = nullptr;

}

float PerspectiveCamera::GenerateRay(const CameraSample &sample, Ray &ray) const {

  Point3f nearP = sample_to_camera_ * Point3f(
      sample.film.x() * inv_output_size_.x(),
      sample.film.y() * inv_output_size_.y(), 0.0f);

  Vector3f d = nearP.normalized();
  float invZ = 1.0f / d.z();


  ray.orig_ = camera_to_world_*Point3f(0,0,0);
  ray.dir_ = camera_to_world_* d;
  ray.mint_ = near_clip_ * invZ;
  ray.maxt_ = far_clip_ * invZ;

  return 1.0f;
}
void PerspectiveCamera::Active() {
  float aspect = output_size_.x() / (float) output_size_.y();

  float recip = 1.0f / (far_clip_ - near_clip_),
      cot = 1.0f / std::tan(deg_to_rad(fov_ / 2.0f));

  Eigen::Matrix4f perspective;
  perspective <<
      cot, 0,   0,   0,
      0, cot,   0,   0,
      0,   0,   far_clip_ * recip, -near_clip_ * far_clip_ * recip,
      0,   0,   1,   0;

  sample_to_camera_ = Transform(
      Eigen::DiagonalMatrix<float, 3>(Vector3f(-0.5f, -0.5f * aspect, 1.0f)) *
          Eigen::Translation<float, 3>(-1.0f, -1.0f/aspect, 0.0f) * perspective).inverse();

  if(filter_== nullptr)
  {
    filter_ = std::dynamic_pointer_cast<Filter>(PhoenixObjectFactory::CreateInstance("MN",PropertyList()));
  }

}
PHOENIX_REGISTER_CLASS(PerspectiveCamera, "perspective");

PHOENIX_NAMESPACE_END