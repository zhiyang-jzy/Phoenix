//
// Created by jzy99 on 2021/1/23.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_BITMAP_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_BITMAP_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/color.h>
#include<phoenix/core/vector.h>


PHOENIX_NAMESPACE_BEGIN
class Bitmap : public Eigen::Array<Color3f, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> {
 public:
  typedef Eigen::Array<Color3f, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Base;

  Bitmap(const string& path);

  Bitmap(const Vector2i &size = Vector2i(0, 0))
      : Base(size.y(), size.x()) { }
  void SaveEXR(const std::string &filename);
  void SavePNG(const string& filename);

};



PHOENIX_NAMESPACE_END


#endif //PHOENIX_INCLUDE_PHOENIX_CORE_BITMAO_H_
