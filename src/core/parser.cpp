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

  Transform transform;

  if(tag==ParserType::PScene)
    node.append_attribute("type")="scene";

  if(tag==ParserType::PTransform)
    transform.setIdentity();

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

        transform = Eigen::Affine3f(trafo) * transform;
      }

      case ParserType::PTransform:{
        prop.SetTransform(node.attribute("name").value(), transform);
        break;
      }

    }
  }
  return result;
}

PHOENIX_NAMESPACE_END