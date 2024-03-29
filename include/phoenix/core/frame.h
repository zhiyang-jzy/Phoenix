//
// Created by jzy99 on 2021/1/21.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_FRAM_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_FRAM_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>
#include<phoenix/core/common.h>

PHOENIX_NAMESPACE_BEGIN

    inline void CoordinateSystem(const Vector3f &a, Vector3f &b, Vector3f &c) {
        if (std::abs(a.x()) > std::abs(a.y())) {
            float invLen = 1.0f / std::sqrt(a.x() * a.x() + a.z() * a.z());
            c = Vector3f(a.z() * invLen, 0.0f, -a.x() * invLen);
        } else {
            float invLen = 1.0f / std::sqrt(a.y() * a.y() + a.z() * a.z());
            c = Vector3f(0.0f, a.z() * invLen, -a.y() * invLen);
        }
        b = c.cross(a);
    }

    class Frame {
    public:
        Vector3f s, t;
        Normal3f n;
    public:
        explicit Frame(Vector3f n) : n(n) {
            CoordinateSystem(n, s, t);
        }

        Frame() = default;

        Vector3f ToLocal(const Vector3f &v) const {
            return Vector3f(
                    v.dot(s), v.dot(t), v.dot(n)
            );
        }

        Vector3f ToWorld(const Vector3f &v) const {
            return s * v.x() + t * v.y() + n * v.z();
        }

        static float CosTheta(const Vector3f &v) {
            return v.z();
        }

        static float SinTheta(const Vector3f &v) {
            float temp = SinTheta2(v);
            if (temp <= 0.0f)
                return 0.0f;
            return std::sqrt(temp);
        }

        static float TanTheta(const Vector3f &v) {
            float temp = 1 - v.z() * v.z();
            if (temp <= 0.0f)
                return 0.0f;
            return std::sqrt(temp) / v.z();
        }

        static float SinTheta2(const Vector3f &v) {
            return 1.0f - v.z() * v.z();
        }


        static float SinPhi(const Vector3f &v) {
            float sinTheta = Frame::SinTheta(v);
            if (sinTheta == 0.0f)
                return 1.0f;
            return Clamp(v.y() / sinTheta, -1.0f, 1.0f);
        }

        static float CosPhi(const Vector3f &v) {
            float sinTheta = Frame::SinTheta(v);
            if (sinTheta == 0.0f)
                return 1.0f;
            return Clamp(v.x() / sinTheta, -1.0f, 1.0f);
        }

        static float sinPhi2(const Vector3f &v) {
            return Clamp(v.y() * v.y() / SinTheta2(v), 0.0f, 1.0f);
        }


        static float cosPhi2(const Vector3f &v) {
            return Clamp(v.x() * v.x() / SinTheta2(v), 0.0f, 1.0f);
        }

    };

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_FRAM_H_
