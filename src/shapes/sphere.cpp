//
// Created by jzy99 on 2021/1/19.
//


#include<phoenix/core/phoenix.h>
#include<phoenix/core/shape.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/pembree.h>
#include<phoenix/core/sampling.h>

PHOENIX_NAMESPACE_BEGIN

class Sphere : public Shape {
 protected:
  Point3f center_;
  float radius_;
 public:
  explicit Sphere(const PropertyList &props) {
    center_ = props.GetPoint("center");
    radius_ = props.GetFloat("radius");
    area_ = 4.0f * PI * radius_ * radius_;
    inv_area_ = 1.0f / area_;
  }
  vector<unsigned int> AddToEmbree(Pembree &embree) const override {
    return {embree.AddSphere(center_, radius_)};
  }

  [[nodiscard]] SampleData SampleSurface(const Point2f& sample) const override {
    auto dir = SquareToUniformSphere(sample);
    SampleData res;

    res.point = center_ + dir * radius_;
    res.normal = (center_ + dir).normalized();
    res.pdf = inv_area_;

    return res;
  }

};

PHOENIX_REGISTER_CLASS(Sphere, "sphere");

PHOENIX_NAMESPACE_END