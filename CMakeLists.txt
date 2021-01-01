# CMakeList.txt: myrender 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
project ("phoenix")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(include)

file ( GLOB PHOENIX_SOURCE src/core/*)

add_library(phoenix_lib STATIC ${PHOENIX_SOURCE})


find_package(spdlog CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)


set(all_package
		spdlog::spdlog
		Eigen3::Eigen
		phoenix_lib
)

add_executable(phoenix "src/main/phoenix.cpp")


target_link_libraries(phoenix ${all_package})




# TODO: 如有需要，请添加测试并安装目标。
