//
// Created by jzy99 on 2021/1/24.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_RENDERER_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_RENDERER_H_



#include<phoenix/core/phoenix.h>
#include<phoenix/core/parser.h>
#include<phoenix/core/scene.h>
#include<phoenix/core/block.h>
#include<filesystem>


PHOENIX_NAMESPACE_BEGIN

class Renderer
{
 public:
  Renderer();;
  void ParseXML(const string& xml_path);
  void Render();
  void OutputEXR();
  void SetThread(int thread_count){thread_count_=thread_count;}

 protected:

  void RenderBlock(shared_ptr<Sampler> sampler, ImageBlock &block);

  shared_ptr<Scene> scene_;
  shared_ptr<Camera> camera_;
  string filename_;
  int thread_count_;
  Point2i output_size_;
  shared_ptr<ImageBlock> result_;
  std::chrono::system_clock::time_point start_time_,stop_time_;


};




PHOENIX_NAMESPACE_END




#endif //PHOENIX_INCLUDE_PHOENIX_CORE_RENDERER_H_
