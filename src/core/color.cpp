//
// Created by jzy99 on 2021/1/28.
//

#include<phoenix/core/color.h>

PHOENIX_NAMESPACE_BEGIN

Color3f Color3f::toSRGB() const {
  Color3f result;

  for (int i = 0; i < 3; ++i) {
    float value = coeff(i);

    if (value <= 0.0031308f)
      result[i] = 12.92f * value;
    else
      result[i] = (1.0f + 0.055f)
          * std::pow(value, 1.0f / 2.4f) - 0.055f;
  }

  return result;
}

float Color3f::getLuminance() const {
  return coeff(0) * 0.212671f + coeff(1) * 0.715160f + coeff(2) * 0.072169f;
}

PHOENIX_NAMESPACE_END

