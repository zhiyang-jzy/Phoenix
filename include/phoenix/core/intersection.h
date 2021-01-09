//
// Created by jzy99 on 2021/1/6.
//

#ifndef PHOENIX_INTERSECTION_H
#define PHOENIX_INTERSECTION_H

#include<phoenix/core/phoenix.h>

PHOENIX_NAMESPACE_BEGIN

class Intersection{
public:
    Point2f uv;
    shared_ptr<Shape> shape;
    Point3f point;
    bool isHit;
    float tfar;


public:

};



PHOENIX_NAMESPACE_END


#endif //PHOENIX_INTERSECTION_H
