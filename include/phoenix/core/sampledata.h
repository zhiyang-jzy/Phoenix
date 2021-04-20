//
// Created by jzy99 on 2021/1/25.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLEDATA_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLEDATA_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>

#include <utility>


PHOENIX_NAMESPACE_BEGIN


    struct SampleData {
        Point3f point;
        Point3f ref;
        Normal3f normal;
        float pdf{};
        SampleData(){}

        SampleData(Point3f _ref) : ref(std::move(_ref)) {}

        SampleData(Point3f _ref, Point3f _p) : ref(std::move(_ref)), point(std::move(_p)) {}
    };


PHOENIX_NAMESPACE_END


#endif //PHOENIX_INCLUDE_PHOENIX_CORE_SAMPLEDATA_H_
