//
// Created by jzy99 on 2021/1/1.
//

#include <phoenix/core/object.h>

PHOENIX_NAMESPACE_BEGIN


std::map<std::string, PhoenixObjectFactory::Constructor> *PhoenixObjectFactory::m_constructors = nullptr;
void PhoenixObjectFactory::registerClass(const std::string &name, const Constructor &constr) {
    if (!m_constructors)
        m_constructors = new std::map<std::string, PhoenixObjectFactory::Constructor>();
    (*m_constructors)[name] = constr;
}


PHOENIX_NAMESPACE_END