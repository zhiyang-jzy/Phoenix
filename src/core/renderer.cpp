//
// Created by jzy99 on 2021/1/24.
//

#include<phoenix/core/renderer.h>
#include<phoenix/core/camera.h>
#include<phoenix/core/block.h>
#include<phoenix/core/sampler.h>
#include<tbb/tbb.h>
#include <phoenix/core/ray.h>
#include<phoenix/core/integrator.h>
#include<iostream>
#include<filesystem>

PHOENIX_NAMESPACE_BEGIN

void Renderer::ParseXML(const string &xml_path) {

  std::filesystem::path t_path(xml_path);
  std::filesystem::current_path(t_path.parent_path());
  spdlog::info(t_path.parent_path().string());
  filename_ = t_path.filename().string();
  SceneParser parser;
  auto t_scene = parser.Parse(filename_);
  scene_ = std::dynamic_pointer_cast<phoenix::Scene>(t_scene);
  camera_ = scene_->camera_;
  output_size_ = camera_->output_size_;
  result_ = make_shared<ImageBlock>(output_size_, camera_->filter_);
}

void Renderer::Render() {
  BlockGenerator blockGenerator(output_size_, PHOENIX_BLOCK_SIZE);
  result_->Clear();
  std::thread render_thread([&] {
    tbb::task_scheduler_init init(thread_count_);
    tbb::blocked_range<int> range(0, blockGenerator.GetBlockCount());
    auto map = [&](const tbb::blocked_range<int> &range) {
      ImageBlock block(Vector2i(PHOENIX_BLOCK_SIZE),
                       camera_->filter_);
      std::shared_ptr<Sampler> sampler(scene_->sampler_->Clone());
      for (int i = range.begin(); i < range.end(); ++i) {
        blockGenerator.Next(block);
        sampler->Prepare(block);
        RenderBlock(sampler, block);
        result_->Put(block);
      }
    };
    tbb::parallel_for(range, map);
  });

  start_time_ = std::chrono::system_clock::now();
  render_thread.join();

  stop_time_ = std::chrono::system_clock::now();
  std::cout<<std::endl;

  spdlog::info("Render complete");
  spdlog::info("Time Token {} seconds",std::chrono::duration_cast<std::chrono::seconds>(stop_time_ - start_time_).count());

}
Renderer::Renderer() {
  scene_ = nullptr;
  thread_count_ = -1;
}
void Renderer::RenderBlock(shared_ptr<Sampler> sampler, ImageBlock &block) {
  Point2i offset = block.GetOffset();
  Vector2i size = block.GetSize();
  auto integrator = scene_->integrator_;
  block.Clear();
  for (int y = 0; y < size.y(); ++y) {
    for (int x = 0; x < size.x(); ++x) {
      for (uint32_t i = 0; i < sampler->GetSampleCount(); ++i) {
        Point2f pixelSample = Point2f((float) (x + offset.x()), (float) (y + offset.y())) + sampler->Next2D();
        Point2f apertureSample = sampler->Next2D();
        CameraSample camera_sample(pixelSample);

        Ray ray;
        float ratio = camera_->GenerateRay(camera_sample, ray);
        Color3f value = integrator->Li(scene_, sampler, ray);
        value *= ratio;
        block.Put(pixelSample, value);

      }
    }
  }
}
void Renderer::OutputEXR() {
  std::shared_ptr<Bitmap> bitmap(result_->ToBitmap());
  std::string outputName = filename_;
  size_t lastdot = outputName.find_last_of('.');
  if (lastdot != std::string::npos)
    outputName.erase(lastdot, std::string::npos);
  bitmap->SaveEXR(outputName);
  bitmap->SavePNG(outputName);
}

PHOENIX_NAMESPACE_END


