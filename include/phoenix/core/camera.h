
#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>
#include<phoenix/core/filter.h>
#include<phoenix/core/vector.h>
#include<phoenix/core/film.h>

PHOENIX_NAMESPACE_BEGIN


struct CameraSample{
  Point2f film;
  CameraSample(Point2f _film):film(_film){}

};

class Camera : public PhoenixObject{

public:

    PClassType GetClassType()const override{return PClassType::PCamera;}

    string ToString()const override{return "Camera";}

    virtual float GenerateRay(const CameraSample& sample,Ray& ray)const = 0;

  void AddChild(shared_ptr<PhoenixObject> child)override;


    //virtual Point2f Sample()
 public:
  Vector2i output_size_;
  shared_ptr<Filter> filter_;
  shared_ptr<Film> film_;

};


PHOENIX_NAMESPACE_END