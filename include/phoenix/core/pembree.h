//
// Created by jzy99 on 2021/1/6.
//

#ifndef PHOENIX_PEMBREE_H
#define PHOENIX_PEMBREE_H


#include <embree3/rtcore.h>
#include <cstdio>
#include <cmath>
#include <limits>
#include <Eigen/Dense>
#include <vector>
#include <phoenix/core/phoenix.h>

#if defined(_WIN32)
#  include <conio.h>
#  include <windows.h>
#endif




#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

PHOENIX_NAMESPACE_BEGIN

class Pembree
{

public:
    RTCDevice device{};
    RTCScene scene;
private:
    void initializeDevice();

public:
    [[nodiscard]] RTCRayHit castRay(const Eigen::Vector3f& origin, const Eigen::Vector3f& dir, float tnear = 1e-3, float tfar = std::numeric_limits<float>::infinity())const;
    unsigned int addMesh(const std::vector<Eigen::Vector3f>& vertices, const std::vector<uint32_t> indices);
    unsigned int addSphere(const Eigen::Vector4f& info);
    void endAdd();
    Pembree();

};

PHOENIX_NAMESPACE_END



#endif //PHOENIX_PEMBREE_H
