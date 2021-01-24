//
// Created by jzy99 on 2021/1/9.
//

#ifndef PHOENIX_FILTER_H
#define PHOENIX_FILTER_H

#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>
#include<phoenix/core/properlist.h>

PHOENIX_NAMESPACE_BEGIN

class Filter : public PhoenixObject{
 public:
  PhoenixObject::PClassType GetClassType()const override{return PhoenixObject::PClassType::PFilter;}
  string ToString()const override{return "";}

  float GetRadius()const{return radius_;}
  virtual float Eval(float x)const = 0;

 protected:
  float radius_;

};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_FILTER_H
