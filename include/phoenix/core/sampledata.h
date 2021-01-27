//
// Created by jzy99 on 2021/1/25.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLEDATA_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLEDATA_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>


PHOENIX_NAMESPACE_BEGIN


struct SampleData
{
  Point3f point;
  Normal3f normal;
  float pdf;
};


PHOENIX_NAMESPACE_END



#endif //PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLEDATA_H_
