//
// Created by jzy99 on 2021/1/19.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_

#include<phoenix/core/phoenix.h>
#include<algorithm>
PHOENIX_NAMESPACE_BEGIN

inline float deg_to_rad(float deg){return deg*(PI/180.f);}
inline float clamp(const float &lo, const float &hi, const float &v) {
  return (std::max)(lo, (std::min)(hi, v));
}

PHOENIX_NAMESPACE_END



#endif //PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_
