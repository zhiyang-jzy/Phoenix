//
// Created by jzy99 on 2021/1/1.
//

#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/color.h>
#include<phoenix/core/transform.h>

PHOENIX_NAMESPACE_BEGIN

class PropertyList {
 public:
  PropertyList() {}

     
  void SetBoolean(const std::string &name, const bool &value);

     
  bool GetBoolean(const std::string &name) const;

     
  bool GetBoolean(const std::string &name, const bool &defaultValue) const;

     
  void SetInteger(const std::string &name, const int &value);

     
  int GetInteger(const std::string &name) const;

     
  int GetInteger(const std::string &name, const int &defaultValue) const;

     
  void SetFloat(const std::string &name, const float &value);

     
  float GetFloat(const std::string &name) const;

     
  float GetFloat(const std::string &name, const float &defaultValue) const;

     
  void SetString(const std::string &name, const std::string &value);

     
  std::string GetString(const std::string &name) const;

     
  std::string GetString(const std::string &name, const std::string &defaultValue) const;

     
  void SetColor(const std::string &name, const Color3f &value);

     
  Color3f GetColor(const std::string &name) const;

     
  Color3f GetColor(const std::string &name, const Color3f &defaultValue) const;

     
  void SetPoint(const std::string &name, const Point3f &value);

     
  Point3f GetPoint(const std::string &name) const;

     
  Point3f GetPoint(const std::string &name, const Point3f &defaultValue) const;

     
  void SetVector(const std::string &name, const Vector3f &value);

     
  Vector3f GetVector(const std::string &name) const;

     
  Vector3f GetVector(const std::string &name, const Vector3f &defaultValue) const;

     
  void SetTransform(const std::string &name, const Transform &value);

     
  Transform GetTransform(const std::string &name) const;

     
  Transform GetTransform(const std::string &name, const Transform &defaultValue) const;
 private:
  /* Custom variant data type (stores one of boolean/integer/float/...) */
  struct Property {
    enum {
      boolean_type, integer_type, float_type,
      string_type, color_type, point_type,
      vector_type, transform_type
    } type;

    struct Value {
      Value() : boolean_value(false) {}
      ~Value() {}
      bool boolean_value;
      int integer_value;
      float float_value;
      std::string string_value;
      Color3f color_value;
      Point3f point_value;
      Vector3f vector_value;
      Transform transform_value;
    } value;

    Property() : type(boolean_type) {}
  };
  std::map<std::string, Property> properties_;
};

PHOENIX_NAMESPACE_END