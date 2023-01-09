//
// Created by jzy99 on 2021/1/23.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_BLOCK_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_BLOCK_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/color.h>
#include<phoenix/core/filter.h>
#include<phoenix/core/vector.h>
#include<tbb/tbb.h>
#include<phoenix/core/bitmap.h>
#include <oneapi/tbb/mutex.h>


PHOENIX_NAMESPACE_BEGIN

class ImageBlock: public Eigen::Array<Color4f,Eigen::Dynamic, Eigen::Dynamic,Eigen::RowMajor>
{
 protected:
  Point2i offset_;
  Vector2i size_;
  float filter_radius_;
  int border_size_;
  float* filter_;
  float* weight_x_,*weight_y_;
  float lookup_factor_;
  mutable tbb::mutex mutex_;


 public:
  ImageBlock(Vector2i size,const shared_ptr<Filter>& filter);
  void SetOffset(const Point2i &offset){offset_=offset;}
  inline const Point2i &GetOffset()const{return offset_;}
  void SetSize(const Point2i& size){size_=size;}
  inline const Vector2i& GetSize()const{return size_;}
  inline int GetBorderSize()const{return border_size_;}
  void Put(ImageBlock&b);
  void Put(const Point2f &pos,const Color3f& value);
  inline void Lock() const { mutex_.lock(); }
  inline void Unlock() const { mutex_.unlock(); }
  void Clear(){setConstant(Color4f());}
  shared_ptr<Bitmap> ToBitmap()const;
  ~ImageBlock(){
    delete[] filter_;
    delete[] weight_x_;
    delete[] weight_y_;
  }

};

class BlockGenerator {
 public:
  BlockGenerator(const Vector2i &size, int blockSize);

  bool Next(ImageBlock &block);
  [[nodiscard]] int GetBlockCount() const { return blocks_left_; }
 protected:
  enum EDirection { ERight = 0, EDown, ELeft, EUp };

  Point2i block_;
  Vector2i num_blocks_;
  Vector2i size_;
  int block_size_;
  int all_blocks_;
  int num_steps_;
  int blocks_left_;
  int steps_left_;
  int direction_;
  tbb::mutex mutex_;
};


PHOENIX_NAMESPACE_END


#endif //PHOENIX_INCLUDE_PHOENIX_CORE_BLOCK_H_
