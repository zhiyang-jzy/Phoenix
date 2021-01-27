
#include<phoenix/core/renderer.h>
#include<cxxopts.hpp>

int main(int argc, char** argv) {

  cxxopts::Options options("phoenix", "A simple renderer");

  options.add_options()
      ("f,file", "XML file", cxxopts::value<std::string>())
      ("n,nthread", "Thread count", cxxopts::value<int>()->default_value("-1"))
      ("h,help", "Print usage")
      ;
  auto result = options.parse(argc, argv);
  if (result.count("help") || result.arguments().empty())
  {
    std::cout << options.help() << std::endl;
    exit(0);
  }
  phoenix::Renderer render;
  render.SetThread(result["nthread"].as<int>());
  render.ParseXML(result["file"].as<std::string>());
  render.Render();
  render.OutputEXR();

}
