//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_SAMPLER_H
#define PHOENIX_SAMPLER_H

#include<phoenix/core/object.h>
#include<phoenix/core/phoenix.h>
#include<phoenix/core/block.h>


PHOENIX_NAMESPACE_BEGIN

class Sampler : public PhoenixObject{

public:
  int sample_count_;
  virtual float Next1D()=0;
  virtual Point2f Next2D()=0;

  virtual shared_ptr<Sampler> Clone()const=0;
  virtual void Prepare(const ImageBlock& block){}
  PhoenixObject::PClassType GetClassType()const override{return PhoenixObject::PClassType::PSampler;}
  string ToString()const override{return "";}
  inline int GetSampleCount()const{return sample_count_;}

};


PHOENIX_NAMESPACE_END

#endif //PHOENIX_SAMPLER_H
