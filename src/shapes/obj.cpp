//
// Created by jzy99 on 2021/1/19.
//

#include<phoenix/core/phoenix.h>
#include<phoenix/core/shape.h>
#include<phoenix/core/properlist.h>
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

PHOENIX_NAMESPACE_BEGIN

class OBJ: public Shape
{
 protected:
  tinyobj::ObjReaderConfig reader_config;
  tinyobj::ObjReader reader;
 public:
  explicit OBJ(const PropertyList& props){
    string filename = props.GetString("filename");
    reader_config.mtl_search_path = "./"; // Path to material files
    reader.ParseFromFile(filename);

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();
  }

  unsigned int AddToEmbree(Pembree& embree)const override{
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();



  }

};



PHOENIX_NAMESPACE_END