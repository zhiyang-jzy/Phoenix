//
// Created by jzy99 on 2021/1/19.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>
#include<algorithm>

PHOENIX_NAMESPACE_BEGIN

    inline float deg_to_rad(float deg) { return deg * (PI / 180.f); }


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

    inline float lerp(float t, float v1, float v2) {
        return ((float) 1 - t) * v1 + t * v2;
    }

    inline string ToLower(const string &value) {
        string result;
        result.resize(value.size());
        std::transform(value.begin(), value.end(), result.begin(), ::tolower);
        return result;
    }

    inline bool EndsWith(const std::string &value, const std::string &ending) {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    inline Point2f SphericalCoordinates(const Vector3f &v) {
        Point2f result(
                std::acos(v.z()),
                std::atan2(v.y(), v.x())
        );
        if (result.y() < 0)
            result.y() += 2*PI;
        return result;
    }

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_COMMON_H_
