//
// Created by jzy99 on 2021/1/3.
//

#include <pugixml.hpp>
#include<map>
#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>
#include <phoenix/core/transform.h>

#pragma once

PHOENIX_NAMESPACE_BEGIN

class SceneParser {
 public:
  SceneParser();
  shared_ptr<PhoenixObject> Parse(const string &path);

 private:
  enum class ParserType{
    PCamera,
    PEmitter,
    PIntegrator,
    PSampler,
    PScene,
    PShape,
    PFilm,


    PFloat,
    PInteger,
    PPoint,
    PLookAt,
    PTransform,
    PInvalid
  };

  shared_ptr<PhoenixObject> ParseTag(pugi::xml_node& node,PropertyList& prop,ParserType parent_tag);
  std::map<string,ParserType> str_to_type_;

  Eigen::Affine3f transform_;



};

PHOENIX_NAMESPACE_END


