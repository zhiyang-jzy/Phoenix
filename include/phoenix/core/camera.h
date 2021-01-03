
#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>


PHOENIX_NAMESPACE_BEGIN

class Camera : public PhoenixObject{

public:

    PClassType getClassType()const override{return PClassType::PCamera;}

    string toString()const override{return "Camera";}

    //virtual Point2f Sample()

protected:
    Vector2i m_outputSize;
};


PHOENIX_NAMESPACE_END