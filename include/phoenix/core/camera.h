
#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>


PHOENIX_NAMESPACE_BEGIN


struct CameraSample{
  Point2f film;

};

class Camera : public PhoenixObject{

public:

    PClassType GetClassType()const override{return PClassType::PCamera;}

    string ToString()const override{return "Camera";}

    virtual float GenerateRay(const CameraSample& sample,Ray& ray)const = 0;



    //virtual Point2f Sample()

protected:
  Transform camera_to_world_;

};


PHOENIX_NAMESPACE_END