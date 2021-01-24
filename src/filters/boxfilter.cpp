//
// Created by jzy99 on 2021/1/23.
//

#include<phoenix/core/phoenix.h>
#include<phoenix/core/filter.h>

PHOENIX_NAMESPACE_BEGIN

class BoxFilter: public Filter{
 public:
  float Eval(float)const override{
    return 1.0f;
  }
 public:
  BoxFilter(const PropertyList& props)
  {
    radius_ = 0.5f;
  }
};

PHOENIX_REGISTER_CLASS(BoxFilter,"box");

class GaussianFilter : public Filter {
 public:
  GaussianFilter(const PropertyList &propList) {
    /* Half filter size */
    radius_ = propList.GetFloat("radius", 2.0f);
    /* Standard deviation of the Gaussian */
    stddev_ = propList.GetFloat("stddev", 0.5f);
  }

  float Eval(float x) const override {
    float alpha = -1.0f / (2.0f * stddev_*stddev_);
    return std::max(0.0f,
                    std::exp(alpha * x * x) -
                        std::exp(alpha * radius_ * radius_));
  }
 protected:
  float stddev_;
};

PHOENIX_REGISTER_CLASS(GaussianFilter,"gaussian");

PHOENIX_NAMESPACE_END
