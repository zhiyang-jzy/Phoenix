//
// Created by jzy99 on 2021/1/19.
//

#include<phoenix/core/phoenix.h>
#include<phoenix/core/shape.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/model.h>
#include<phoenix/core/pembree.h>

PHOENIX_NAMESPACE_BEGIN

class OBJ: public Shape
{
 protected:
  Model model;
 public:
  explicit OBJ(const PropertyList& props){
    string filename = props.GetString("filename");
    model.Load(filename);

  }

  vector<unsigned int> AddToEmbree(Pembree& embree)const override{
    vector<unsigned int> res;
    for (auto & mesh : model.meshes)
    {
      std::vector<Eigen::Vector3f> temp;
      for(auto & vertex : mesh->vertices)
      {
        temp.push_back(vertex.position);
      }
//	    std::cout<<"add "<<meshe.vertices.size()<<std::endl;
      unsigned int id = embree.AddMesh(temp, mesh->indices);
      res.push_back(id);
    }
    return res;
  }

};

PHOENIX_REGISTER_CLASS(OBJ,"obj");


PHOENIX_NAMESPACE_END