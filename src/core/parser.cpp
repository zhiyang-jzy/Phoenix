//
// Created by jzy99 on 2021/1/3.
//

#include<phoenix/core/parser.h>
#include<filesystem>
#include<iostream>
#include<phoenix/core/parsertool.h>
#include<phoenix/core/properlist.h>

PHOENIX_NAMESPACE_BEGIN

shared_ptr<PhoenixObject> phoenix::SceneParser::Parse(const string &path) {
  std::filesystem::path local_path(path);
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(local_path.c_str());

  if(!result)
  {
    spdlog::error("parse xml faild");
    exit(0);
  }

  PropertyList props;
  auto res = ParseTag(*doc.begin(),props,ParserType::PInvalid);
  return res;




}
phoenix::SceneParser::SceneParser() {
  str_to_type_["float"] = ParserType::PFloat;
  str_to_type_["integer"] = ParserType::PInteger;
  str_to_type_["transform"] = ParserType::PTransform;
  str_to_type_["lookat"] = ParserType::PLookAt;
  str_to_type_["scene"] = ParserType::PScene;
  str_to_type_["camera"] = ParserType::PCamera;
  str_to_type_["shape"] = ParserType::PShape;
  str_to_type_["point"] = ParserType::PPoint;
  str_to_type_["string"] = ParserType::PString;
  str_to_type_["sampler"] = ParserType::PSampler;
  str_to_type_["integrator"] = ParserType::PIntegrator;
  str_to_type_["color"] = ParserType::PColor;
  str_to_type_["filter"] = ParserType::PFilter;
  str_to_type_["emitter"] = ParserType::PEmitter;
  str_to_type_["bsdf"] = ParserType::PBSDF;
  str_to_type_["translate"] = ParserType::PTranslate;
  str_to_type_["scale"] = ParserType::PScale;

}
shared_ptr<PhoenixObject> phoenix::SceneParser::ParseTag(pugi::xml_node& node,PropertyList& prop,ParserType parent_tag) {

  auto test = node.name();
  auto it = str_to_type_.find(node.name());

  auto tag = it->second;
  bool has_parent = parent_tag!=ParserType::PInvalid;
  bool parent_is_object = has_parent && tag<ParserType::PFloat;
  bool current_is_object = tag<ParserType::PFloat;
  bool parent_is_transform = parent_tag==ParserType::PTransform;
  bool current_is_transform_op = false;

  if (node.type() == pugi::node_comment || node.type() == pugi::node_declaration)
    return nullptr;

  if(tag==ParserType::PScene)
    node.append_attribute("type")="scene";

  if(tag==ParserType::PTransform)
    transform_.setIdentity();

  PropertyList prop_list;

  vector<shared_ptr<PhoenixObject>> children;

  for(pugi::xml_node& ch:node.children()){
    shared_ptr<PhoenixObject> child = ParseTag(ch,prop_list,tag);
    if(child!= nullptr)
      children.push_back(child);
  }

  shared_ptr<PhoenixObject> result = nullptr;

  if(current_is_object)
  {
    auto test1 = node.attribute("type").value();
    result = PhoenixObjectFactory::CreateInstance(node.attribute("type").value(),prop_list);

    for(auto& ch:children){
      result->AddChild(ch);
      ch->SetParent(result);
    }

    result->Active();
  }
  else{
    switch (tag) {
      case ParserType::PFloat : {
        auto test3 = node.attribute("name").value();
        prop.SetFloat(node.attribute("name").value(), str_to_float(node.attribute("value").value()));
        break;
      }
      case ParserType::PLookAt:{
        Eigen::Vector3f origin = str_to_vector3f(node.attribute("origin").value());
        Eigen::Vector3f target = str_to_vector3f(node.attribute("target").value());
        Eigen::Vector3f up = str_to_vector3f(node.attribute("up").value());

        Vector3f dir = (target - origin).normalized();
        Vector3f left = up.normalized().cross(dir).normalized();
        Vector3f newUp = dir.cross(left).normalized();

        Eigen::Matrix4f trafo;
        trafo << left, newUp, dir, origin,
            0, 0, 0, 1;


        transform_ = Eigen::Affine3f(trafo) * transform_;

        break;
      }
      case ParserType::PTransform:{
        prop.SetTransform(node.attribute("name").value(), transform_.matrix());
        break;
      }

      case ParserType::PTranslate:{
        auto v = str_to_vector3f(node.attribute("value").value());
        transform_ =  Eigen::Translation<float, 3>(v.x(), v.y(), v.z()) * transform_;
        break;
      };

      case ParserType::PScale:{
        Eigen::Vector3f v = str_to_vector3f(node.attribute("value").value());
        transform_ = Eigen::DiagonalMatrix<float, 3>(v) * transform_;
        break;
      }
      case ParserType::PPoint:{
        prop.SetPoint(node.attribute("name").value(), str_to_point3f(node.attribute("value").value()));
        break;
      }

      case ParserType::PInteger:{
        prop.SetInteger(node.attribute("name").value(), str_to_integer(node.attribute("value").value()));
        break;
      }
      case ParserType::PString:{
        prop.SetString(node.attribute("name").value(), node.attribute("value").value());
        break;
      }
      case ParserType::PColor:{
        prop.SetColor(node.attribute("name").value(), str_to_vector3f(node.attribute("value").value()).array());
        break;
      }

    }
  }
  return result;
}

PHOENIX_NAMESPACE_END