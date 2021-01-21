//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_SAMPLER_H
#define PHOENIX_SAMPLER_H

#include<phoenix/core/object.h>
#include<phoenix/core/phoenix.h>


PHOENIX_NAMESPACE_BEGIN

class Sampler : public PhoenixObject{

public:
  virtual float Next1D()=0;
  virtual Point2f Next2D()=0;
  PhoenixObject::PClassType GetClassType()const override{return PhoenixObject::PClassType::PSampler;}
  string ToString()const override{return "";}

};


PHOENIX_NAMESPACE_END

#endif //PHOENIX_SAMPLER_H
