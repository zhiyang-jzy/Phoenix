//
// Created by jzy99 on 2021/4/12.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_TEXTURE_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_TEXTURE_H_
#include<phoenix/core/phoenix.h>
#include<phoenix/core/color.h>
#include "vector.h"

PHOENIX_NAMESPACE_BEGIN

class Texture{
 public:
  virtual Color3f GetColor(Point2f uv)const{
    return {0.0f};
  }
};

class ImageTexture : public Texture {

 private:
  unsigned char *data;
  int width, height;
  int bytes_per_scanline;
 public:
  const static int bytes_per_pixel = 3;

  ImageTexture()
      : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}
  ImageTexture(unsigned char* _data,int width,int height,int bytes_per_scanline);

  ~ImageTexture() {
    delete data;
  }

  [[nodiscard]] Color3f GetColor(Point2f uv)const override;


};


PHOENIX_NAMESPACE_END



#endif //PHOENIX_INCLUDE_PHOENIX_CORE_TEXTURE_H_
