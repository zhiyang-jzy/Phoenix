#include<iostream>


#include <spdlog/spdlog.h>
#include<tfm/tinyformat.h>
#include<phoenix/cameras/perspective.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/scene.h>
#include<phoenix/core/parser.h>
#include<phoenix/core/ray.h>
#include<phoenix/core/ppm.h>
#include<phoenix/core/interaction.h>
#include<phoenix/core/integrator.h>
#include<pcg/pcg32.h>
#include<iostream>

#include<filesystem>


int main()
{
//  phoenix::PropertyList p;
//  p.SetPoint("look_from",phoenix::Point3f(0,1,0));
//  p.SetPoint("look_at",phoenix::Point3f(0,0,0));
//  p.SetVector("look_up",phoenix::Vector3f(0,0,1));
//  p.SetFloat("focal_length",0.1);
//
//  auto film = make_shared<phoenix::Film>(p);
//  film->full_resolution_ = phoenix::Point2i(800,800);
//
//  auto a = phoenix::PhoenixObjectFactory::CreateInstance("perspective",p);
//  a->AddChild(std::dynamic_pointer_cast<phoenix::PhoenixObject>(film));
//
//
//  shared_ptr<phoenix::PerspectiveCamera> r = std::dynamic_pointer_cast<phoenix::PerspectiveCamera>(a);
//  r->Active();

  phoenix::SceneParser parser;
  shared_ptr<phoenix::PhoenixObject> t_scene =  parser.Parse(R"(C:\Users\jzy99\Desktop\test.xml)");
  shared_ptr<phoenix::Scene> scene = std::dynamic_pointer_cast<phoenix::Scene>(t_scene);


  int width = scene->camera_->output_size_.x(),height = scene->camera_->output_size_.y();
  phoenix::PPM ppm("test",width,height);
  spdlog::info("{} {}",width,height);

  for(int i=0;i<width;i++)
  {
    for(int j=0;j<height;j++)
    {
      phoenix::Ray ray;
      phoenix::Vector2f vec(i,j);
      phoenix::CameraSample sample{vec};
      phoenix::Interaction it;
      scene->camera_->GenerateRay(sample,ray);
      auto color = scene->integrator_->Li(scene,scene->sampler_,ray);
      ppm.setpixel(i,j,Eigen::Vector3f(color.x(),color.y(),color.z()));
    }
  }
  ppm.write_file();
  phoenix::Vector3f r(0,1,0);

  int a = 3;

}



//using namespace std;
//using namespace std::filesystem;
//
//void search(tinyxml2::XMLNode* node)
//{
//    if(node== nullptr)
//    {
//        return;
//    }
//
//    if(node->ToComment())
//    {
//        return;
//    }
//    std::cout<<" === "<<node->Value()<<"\n";
//    auto att = node->ToElement();
//
//
//    if(att!= nullptr &&  att->FirstAttribute()!= nullptr)
//    {
//        auto sub_att = att->FirstAttribute();
//        while(sub_att!= nullptr)
//
//        {
//            std::cout<< sub_att->Name()<<":"<< sub_att->Value()<<" -- \n";
//            sub_att = sub_att->Next();
//        }
//    }
//
//    cout<<std::endl;
//    tinyxml2::XMLNode* child = node->FirstChild();
//    while(child!= nullptr)
//    {
//        search(child);
//        child=child->NextSibling();
//    }
//
//}


//path a(R"(C:\Users\jzy99\Desktop\test.xml)");
//spdlog::info(a.generic_string());
//tinyxml2::XMLDocument doc;
//doc.LoadFile(a.generic_string().c_str());
//auto root_elec = doc.RootElement();            //等同于不指定参数的FirstChildElement()
//
//search(root_elec);