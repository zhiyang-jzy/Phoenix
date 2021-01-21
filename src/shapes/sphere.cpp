//
// Created by jzy99 on 2021/1/19.
//


#include<phoenix/core/phoenix.h>
#include<phoenix/core/shape.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/pembree.h>

PHOENIX_NAMESPACE_BEGIN


class Sphere: public Shape{
 protected:
  Point3f center_;
  float radius_;
 public:
  explicit Sphere(const PropertyList& props){
    center_ = props.GetPoint("center");
    radius_ = props.GetFloat("radius");
 }
 vector<unsigned int> AddToEmbree(Pembree& embree)const override{
    return {embree.AddSphere(center_,radius_)};
  }

};

PHOENIX_REGISTER_CLASS(Sphere,"sphere");




PHOENIX_NAMESPACE_END