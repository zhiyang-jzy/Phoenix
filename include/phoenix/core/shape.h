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
PHOENIX_NAMESPACE_BEGIN

class Shape : public PhoenixObject {
 private:
  shared_ptr<BSDF> bsdf_;
  Transform object_to_world_,world_to_object_;
 public:
  [[nodiscard]] shared_ptr<BSDF> GetBSDF()const{return bsdf_;}
  [[nodiscard]] PClassType GetClassType()const override{return PClassType::PShape;}
  [[nodiscard]] string ToString() const override{return "shape";}
  virtual unsigned int AddToEmbree(Pembree& embree)const =0;

};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_SHAPE_H
