//
// Created by jzy99 on 2021/1/13.
//

#include<phoenix/core/properlist.h>
#include<spdlog/spdlog.h>

PHOENIX_NAMESPACE_BEGIN

#define DEFINE_PROPERTY_ACCESSOR(Type, TypeName, XmlName) \
    void PropertyList::Set##TypeName(const std::string &name, const Type &value) { \
        if (properties_.find(name) != properties_.end())  \
             spdlog::info("{} not find",name);             \
        auto &prop = properties_[name]; \
        prop.value.XmlName##_value = value; \
        prop.type = Property::XmlName##_type; \
    } \
    \
    Type PropertyList::Get##TypeName(const std::string &name) const { \
        auto it = properties_.find(name); \
        return it->second.value.XmlName##_value; \
    } \
    \
    Type PropertyList::Get##TypeName(const std::string &name, const Type &defVal) const { \
        auto it = properties_.find(name); \
        if(it==properties_.end()) return defVal;\
        return it->second.value.XmlName##_value; \
    }

DEFINE_PROPERTY_ACCESSOR(bool, Boolean, boolean)
DEFINE_PROPERTY_ACCESSOR(int, Integer, integer)
DEFINE_PROPERTY_ACCESSOR(float, Float, float)
DEFINE_PROPERTY_ACCESSOR(Color3f, Color, color)
DEFINE_PROPERTY_ACCESSOR(Point3f, Point, point)
DEFINE_PROPERTY_ACCESSOR(Vector3f, Vector, vector)
DEFINE_PROPERTY_ACCESSOR(std::string, String, string)
DEFINE_PROPERTY_ACCESSOR(Transform, Transform, transform)

bool PropertyList::HasVal(const string &name) const {
  return properties_.count(name) > 0;
}

PHOENIX_NAMESPACE_END

