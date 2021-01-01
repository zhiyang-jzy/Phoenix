#include<iostream>


#include <spdlog/spdlog.h>

#include <phoenix/core/properlist.h>
#include <phoenix/core/camera.h>
#include <phoenix/core/object.h>

using namespace std;

int main()
{
    spdlog::info("test");

    phoenix::PropertyList a;

    phoenix::Camera r(a);
    auto ss = phoenix::PhoenixObjectFactory::createInstance("camera",a);
    spdlog::info(ss->toString());
    spdlog::info(r.toString());
}