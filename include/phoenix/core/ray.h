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
    Ray(const Point3f& orig,const Point3f& dir):orig_(orig),dir_(dir){}

};


PHOENIX_NAMESPACE_END



#endif //PHOENIX_RAY_H
