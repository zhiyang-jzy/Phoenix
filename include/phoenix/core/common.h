//
// Created by jzy99 on 2021/1/19.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_

#include<phoenix/core/phoenix.h>
#include<algorithm>
PHOENIX_NAMESPACE_BEGIN

inline float deg_to_rad(float deg) { return deg * (PI / 180.f); }
inline float clamp(const float &lo, const float &hi, const float &v) {
  return (std::max)(lo, (std::min)(hi, v));
}
inline float Fresnel(float cosThetaI, float extIOR, float intIOR) {
  float etaI = extIOR, etaT = intIOR;

  if (extIOR == intIOR)
    return 0.0f;
  if (cosThetaI < 0.0f) {
    std::swap(etaI, etaT);
    cosThetaI = -cosThetaI;
  }
  float eta = etaI / etaT,
      sinThetaTSqr = eta * eta * (1 - cosThetaI * cosThetaI);

  if (sinThetaTSqr > 1.0f)
    return 1.0f;

  float cosThetaT = std::sqrt(1.0f - sinThetaTSqr);

  float Rs = (etaI * cosThetaI - etaT * cosThetaT)
      / (etaI * cosThetaI + etaT * cosThetaT);
  float Rp = (etaT * cosThetaI - etaI * cosThetaT)
      / (etaT * cosThetaI + etaI * cosThetaT);

  return (Rs * Rs + Rp * Rp) / 2.0f;
}

inline float Clamp(float value, float min, float max) {
  if (value < min)
    return min;
  else if (value > max)
    return max;
  else return value;
}

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_
