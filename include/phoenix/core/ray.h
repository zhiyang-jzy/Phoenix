//
// Created by jzy99 on 2021/1/6.
//

#ifndef PHOENIX_RAY_H
#define PHOENIX_RAY_H

#include<phoenix/core/phoenix.h>

PHOENIX_NAMESPACE_BEGIN

class Ray
{

public:
    Point3f orig_;
    Vector3f dir_;
    float mint_;
    float maxt_;
    Ray(const Point3f& orig,const Point3f& dir):orig_(orig),dir_(dir){}
    Ray()=default;

    Point3f At(float time)const{return orig_+dir_*time;}

};


PHOENIX_NAMESPACE_END



#endif //PHOENIX_RAY_H
