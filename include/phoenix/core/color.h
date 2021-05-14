//
// Created by jzy99 on 2021/1/1.
//
#pragma once

#include<phoenix/core/phoenix.h>
#include<Eigen/Dense>

PHOENIX_NAMESPACE_BEGIN

struct Color3f : public Eigen::Array3f {
 public:
  typedef Eigen::Array3f Base;

   
  Color3f(float value = 0.f) : Base(value, value, value) { }

   
  Color3f(float r, float g, float b) : Base(r, g, b) { }

   
  template <typename Derived> Color3f(const Eigen::ArrayBase<Derived>& p)
      : Base(p) { }

   
  template <typename Derived> Color3f &operator=(const Eigen::ArrayBase<Derived>& p) {
    this->Base::operator=(p);
    return *this;
  }

   
  float &r() { return x(); }
   
  const float &r() const { return x(); }
   
  float &g() { return y(); }
   
  const float &g() const { return y(); }
   
  float &b() { return z(); }
   
  const float &b() const { return z(); }

   
  Color3f clamp() const { return Color3f(std::max(r(), 0.0f),
                                         std::max(g(), 0.0f), std::max(b(), 0.0f)); }

   
  bool isValid() const{
    for (int i=0; i<3; ++i) {
      float value = coeff(i);
      if (value < 0 || !std::isfinite(value))
        return false;
    }
    return true;
  }

   
  Color3f toLinearRGB() const;

   
  Color3f toSRGB() const;

   
  float getLuminance() const;

   
};

/**
 * \brief Represents a linear RGB color and a weight
 *
 * This is used by Nori's image reconstruction filter code
 */
struct Color4f : public Eigen::Array4f {
 public:
  typedef Eigen::Array4f Base;

   
  Color4f() : Base(0.0f, 0.0f, 0.0f, 0.0f) { }

   
  Color4f(const Color3f &c) : Base(c.r(), c.g(), c.b(), 1.0f) { }

   
  Color4f(float r, float g, float b, float w) : Base(r, g, b, w) { }

   
  template <typename Derived> Color4f(const Eigen::ArrayBase<Derived>& p)
      : Base(p) { }

   
  template <typename Derived> Color4f &operator=(const Eigen::ArrayBase<Derived>& p) {
    this->Base::operator=(p);
    return *this;
  }

   
  Color3f divideByFilterWeight() const {
    if (w() != 0)
      return head<3>() / w();
    else
      return Color3f(0.0f);
  }

   
};

PHOENIX_NAMESPACE_END