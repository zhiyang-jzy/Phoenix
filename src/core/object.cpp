//
// Created by jzy99 on 2021/1/1.
//

#include <phoenix/core/object.h>

PHOENIX_NAMESPACE_BEGIN


std::map<std::string, PhoenixObjectFactory::Constructor> *PhoenixObjectFactory::constructors_ = nullptr;
void PhoenixObjectFactory::RegisterClass(const std::string &name, const Constructor &constr) {
    if (!constructors_)
        constructors_ = new std::map<std::string, PhoenixObjectFactory::Constructor>();
    (*constructors_)[name] = constr;
}


PHOENIX_NAMESPACE_END