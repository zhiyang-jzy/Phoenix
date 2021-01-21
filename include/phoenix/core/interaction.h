//
// Created by jzy99 on 2021/1/6.
//

#ifndef PHOENIX_Interaction_H
#define PHOENIX_Interaction_H

#include<phoenix/core/phoenix.h>

PHOENIX_NAMESPACE_BEGIN

struct Interaction {
 public:
  Point2f uv;
  shared_ptr<Shape> shape;
  Point3f point;
  Vector3f normal;
  bool isHit;
  float tfar;
  unsigned geoID, primID;

 public:

};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_Interaction_H
