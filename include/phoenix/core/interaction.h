//
// Created by jzy99 on 2021/1/6.
//

#ifndef PHOENIX_INTERSECTION_H
#define PHOENIX_INTERSECTION_H

#include<phoenix/core/phoenix.h>

PHOENIX_NAMESPACE_BEGIN

class Intersection{
public:
    Point2f uv_;
    shared_ptr<Shape> shape_;
    Point3f point_;
    bool isHit_;
    float tfar_;


public:

};



PHOENIX_NAMESPACE_END


#endif //PHOENIX_INTERSECTION_H
