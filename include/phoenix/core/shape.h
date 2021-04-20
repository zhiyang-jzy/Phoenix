//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_SHAPE_H
#define PHOENIX_SHAPE_H

#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>
#include<phoenix/core/bsdf.h>
#include<phoenix/core/phoenix.h>
#include<phoenix/core/transform.h>
#include<phoenix/core/sampledata.h>
#include<phoenix/core/vector.h>
#include<phoenix/core/emitter.h>
PHOENIX_NAMESPACE_BEGIN

class Shape : public PhoenixObject {
 protected:
  shared_ptr<BSDF> bsdf_;
  shared_ptr<Emitter> emitter_;
  float area_, inv_area_;
  Transform object_to_world_, world_to_object_;
 public:
  [[nodiscard]] shared_ptr<BSDF> GetBSDF() const { return bsdf_; }
  [[nodiscard]] PClassType GetClassType() const override { return PClassType::PShape; }
  [[nodiscard]] string ToString() const override { return "shape"; }
  virtual vector<unsigned int> AddToEmbree(Pembree &embree) = 0;
  [[nodiscard]] float GetArea() const { return area_; };
  [[nodiscard]] virtual SampleData SampleSurface(const Point2f &sample) const = 0;
  [[nodiscard]] virtual float PdfSurface(const SampleData& sRec)const =0;
  void AddChild(shared_ptr<PhoenixObject> child) override;
  virtual Color3f GetTextureColor(unsigned int geoid,unsigned int priid, Point2f uv) const { return {0.0f}; };
  shared_ptr<Emitter> GetEmitter() const { return emitter_; }

  void Active() override;
  bool IsEmitter() { return emitter_ != nullptr; }

};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_SHAPE_H
