//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_EMITTER_H
#define PHOENIX_EMITTER_H

#include <phoenix/core/object.h>
#include <phoenix/core/properlist.h>

#include <utility>
#include "ray.h"

PHOENIX_NAMESPACE_BEGIN

struct EmitterQueryRecord {
  /// Origin point from which we sample the emitter
  Point3f ref;
  /// Sampled point on the emitter
  Point3f p;
  /// Normal at the emitter point
  Normal3f n;
  /// Direction between the hit point and the emitter point
  Vector3f wi;
  /// Probability
  float pdf{};
  /// Shadow ray
  Ray shadowRay;

  /// Create an unitialized query record
  EmitterQueryRecord() = default;

  /// Create a new query record that can be used to sample a emitter
  explicit EmitterQueryRecord(Point3f  ref) : ref(std::move(ref)) { pdf=.0f;}

  /**
   * \brief Create a query record that can be used to query the
   * sampling density after having intersected an area_ emitter
   */
  EmitterQueryRecord(const Point3f& ref, const Point3f& p, Normal3f  n) :
      ref(ref), p(p), n(std::move(n)) {
    pdf=.0f;
    wi = (p - ref).normalized();
  }
};


class Emitter : public PhoenixObject {

 protected:
  shared_ptr<Shape> shape_;

 public:
  [[nodiscard]] PClassType GetClassType() const override { return PClassType::PEmitter; }

  [[nodiscard]] string ToString() const override { return "emitter"; }

  void SetMesh(const shared_ptr<Shape>& shape){shape_=shape; spdlog::info("set emitter father");}

  virtual Color3f Sample(EmitterQueryRecord& IRec, const Point2f& sample) const = 0;

  [[nodiscard]] virtual Color3f Eval(const EmitterQueryRecord& lRec) const = 0;


  [[nodiscard]] virtual float Pdf(const EmitterQueryRecord& lRec) const = 0;

  void SetParent(shared_ptr<PhoenixObject> father)override{
//    shape_ = std::dynamic_pointer_cast<Shape>(father);
//    spdlog::info("set shape");
  }

};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_EMITTER_H
