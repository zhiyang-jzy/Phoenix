//
// Created by jzy99 on 2021/1/23.
//

#include<phoenix/core/block.h>
#include<spdlog/spdlog.h>
#include <utility>
#include<iostream>

PHOENIX_NAMESPACE_BEGIN

ImageBlock::ImageBlock(phoenix::Vector2i size, const shared_ptr<Filter> &filter) : offset_(0, 0), size_(std::move(size)) {
  filter_radius_ = filter->GetRadius();
  border_size_ = (int) std::ceil(filter_radius_ - 0.5f);
  filter_ = new float[PHOENIX_FILTER_RESOLUTION + 1];
  for (int i = 0; i < PHOENIX_FILTER_RESOLUTION; ++i) {
    float pos = (filter_radius_ * i) / PHOENIX_FILTER_RESOLUTION;
    filter_[i] = filter->Eval(pos);
  }
  filter_[PHOENIX_FILTER_RESOLUTION] = 0.0f;
  lookup_factor_ = PHOENIX_FILTER_RESOLUTION / filter_radius_;
  int weightSize = (int) std::ceil(2 * filter_radius_) + 1;
  weight_x_ = new float[weightSize];
  weight_y_ = new float[weightSize];
  memset(weight_x_, 0, sizeof(float) * weightSize);
  memset(weight_y_, 0, sizeof(float) * weightSize);

  resize(size.y() + 2*border_size_, size.x() + 2*border_size_);
}
void ImageBlock::Put(ImageBlock& b) {
  Vector2i offset = b.GetOffset() - offset_ +
      Vector2i::Constant(border_size_ - b.GetBorderSize());
  Vector2i size   = b.GetSize()   + Vector2i(2*b.GetBorderSize());

  tbb::mutex::scoped_lock lock(mutex_);

  block(offset.y(), offset.x(), size.y(), size.x())
      += b.topLeftCorner(size.y(), size.x());
}
void ImageBlock::Put(const Point2f &_pos, const Color3f &value) {
  Point2f pos(
      _pos.x() - 0.5f - (offset_.x() - border_size_),
      _pos.y() - 0.5f - (offset_.y() - border_size_)
  );

  Point2i min_t((int)  std::ceil(pos.x() - filter_radius_), (int)  std::ceil(pos.y() - filter_radius_)),
  max_t((int) std::floor(pos.x() + filter_radius_), (int) std::floor(pos.y() + filter_radius_));

  Point2i min_t1(0,0),max_t2((int)cols()-1,(int)rows()-1);

  Point2i start=min_t.cwiseMax(min_t1),end=max_t.cwiseMin(max_t2);

  for (int x=start.x(), idx = 0; x<=end.x(); ++x)
    weight_x_[idx++] = filter_[(int) (std::abs(x-pos.x()) * lookup_factor_)];
  for (int y=start.y(), idx = 0; y<=end.y(); ++y)
    weight_y_[idx++] = filter_[(int) (std::abs(y-pos.y()) * lookup_factor_)];

  for (int y=start.y(), yr=0; y<=end.y(); ++y, ++yr)
    for (int x=start.x(), xr=0; x<=end.x(); ++x, ++xr)
      coeffRef(y, x) += Color4f(value) * weight_x_[xr] * weight_y_[yr];
}
shared_ptr<Bitmap> ImageBlock::ToBitmap() const {
  auto result = make_shared<Bitmap>(size_);
  for (int y=0; y<size_.y(); ++y)
    for (int x=0; x<size_.x(); ++x)
      result->coeffRef(y, x) = coeff(y + border_size_, x + border_size_).divideByFilterWeight();
  return result;

}
BlockGenerator::BlockGenerator(const phoenix::Vector2i &size, int blockSize):size_(size),block_size_(blockSize) {

  num_blocks_ = Vector2i(
      (int) std::ceil(size.x() / (float) blockSize),
      (int) std::ceil(size.y() / (float) blockSize));
  all_blocks_ = num_blocks_.x() * num_blocks_.y();
  blocks_left_ = num_blocks_.x() * num_blocks_.y();
  direction_ = ERight;
  block_ = Point2i(num_blocks_ / 2);
  steps_left_ = 1;
  num_steps_ = 1;

}
bool BlockGenerator::Next(ImageBlock &block) {
  tbb::mutex::scoped_lock lock(mutex_);
  std::cerr << "\rremaining: " << (float)blocks_left_/(float)all_blocks_ << ' ' << std::flush;

  if (blocks_left_ == 0)
    return false;

  Point2i pos = block_ * block_size_;
  block.SetOffset(pos);
  block.SetSize((size_ - pos).cwiseMin(Vector2i::Constant(block_size_)));

  if (--blocks_left_ == 0)
    return true;

  do {
    switch (direction_) {
      case ERight: ++block_.x(); break;
      case EDown:  ++block_.y(); break;
      case ELeft:  --block_.x(); break;
      case EUp:    --block_.y(); break;
    }

    if (--steps_left_ == 0) {
      direction_ = (direction_ + 1) % 4;
      if (direction_ == ELeft || direction_ == ERight)
        ++num_steps_;
      steps_left_ = num_steps_;
    }
  } while ((block_.array() < 0).any() ||
      (block_.array() >= num_blocks_.array()).any());

  return true;
}
PHOENIX_NAMESPACE_END

