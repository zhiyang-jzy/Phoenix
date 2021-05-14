# CMakeList.txt: myrender 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

project(PHOENIX)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if(APPLE)
    set(embree_DIR  /Users/yang/local/embree3/lib/cmake/embree-3.12.1)
endif()



include_directories(include)

file(GLOB PHOENIX_SOURCE src/core/* src/cameras/* src/shapes/* src/samplers/* src/integrators/* src/filters/* src/emitters/* src/bsdfs/* src/textures/*
        )

find_package(spdlog CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


find_package(pugixml CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED tbb)
find_package(assimp CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)

if(APPLE)
    find_package(embree 3.12.1 CONFIG REQUIRED)
else()
    find_package(embree 3.0  CONFIG REQUIRED)
endif()


find_package(OpenEXR CONFIG REQUIRED)


set(all_package spdlog::spdlog Eigen3::Eigen embree pugixml TBB::tbb assimp::assimp  OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig cxxopts::cxxopts imgui::imgui)

add_executable(phoenix "src/main/phoenix.cpp" ${PHOENIX_SOURCE})

target_link_libraries(phoenix ${all_package})


