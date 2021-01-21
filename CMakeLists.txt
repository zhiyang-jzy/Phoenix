# CMakeList.txt: myrender 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
project ("phoenix")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)


if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif(CMAKE_TOOLCHAIN_FILE)

include_directories(include)

file ( GLOB PHOENIX_SOURCE src/core/* src/cameras/* src/shapes/*

        )

find_package(spdlog CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(embree 3.0 REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

set(all_package spdlog::spdlog Eigen3::Eigen tinyxml2::tinyxml2 embree pugixml  TBB::tbb tinyobjloader::tinyobjloader)

add_executable(phoenix "src/main/phoenix.cpp" ${PHOENIX_SOURCE})

target_link_libraries(phoenix ${all_package})


