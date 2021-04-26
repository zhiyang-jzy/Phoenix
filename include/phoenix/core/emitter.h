//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_EMITTER_H
#define PHOENIX_EMITTER_H

#include <phoenix/core/object.h>
#include <phoenix/core/properlist.h>

#include <utility>
#include "ray.h"

PHOENIX_NAMESPACE_BEGIN

    struct EmitterQueryRecord {

        Point3f ref;

        Point3f p;

        Normal3f n;

        Vector3f wi;

        float pdf{};

        Ray shadowRay;

        EmitterQueryRecord() = default;

        explicit EmitterQueryRecord(Point3f ref) : ref(std::move(ref)) { pdf = .0f; }

        EmitterQueryRecord(const Point3f &ref, const Point3f &p, Normal3f n) :
                ref(ref), p(p), n(std::move(n)) {
            pdf = .0f;
            wi = (p - ref).normalized();
        }
    };


    class Emitter : public PhoenixObject {

    public:
        enum class EmitterType {
            AREA, ENV, POINT
        };

    protected:
        shared_ptr<Shape> shape_;

    public:
        [[nodiscard]] PClassType GetClassType() const override { return PClassType::PEmitter; }

        [[nodiscard]] virtual EmitterType GetEmitterType() const = 0;

        [[nodiscard]] string ToString() const override { return "emitter"; }

        void SetShape(const shared_ptr<Shape> &shape) {
            shape_ = shape;
            spdlog::info("set emitter father");
        }

        [[nodiscard]] const shared_ptr<Shape> &GetShape() const { return shape_; }

        virtual Color3f Sample(EmitterQueryRecord &IRec, const Point2f &sample) const = 0;

        [[nodiscard]] virtual Color3f Eval(const EmitterQueryRecord &lRec) const = 0;

        [[nodiscard]] virtual float Pdf(const EmitterQueryRecord &lRec) const = 0;

        [[nodiscard]] virtual Color3f GetColor() const { return Color3f(0.0f); }


        void SetParent(shared_ptr<PhoenixObject> father) override {
//    shape_ = std::dynamic_pointer_cast<Shape>(father);
//    spdlog::info("set shape");
        }

    };

PHOENIX_NAMESPACE_END

#endif //PHOENIX_EMITTER_H
