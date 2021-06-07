//
// Created by jzy99 on 2021/1/23.
//

#include<phoenix/core/phoenix.h>
#include<phoenix/core/filter.h>

PHOENIX_NAMESPACE_BEGIN

    class BoxFilter : public Filter {
    public:
        float Eval(float) const override {
            return 1.0f;
        }

    public:
        BoxFilter(const PropertyList &props) {
            radius_ = 0.5f;
        }
    };

    PHOENIX_REGISTER_CLASS(BoxFilter, "box");

    class GaussianFilter : public Filter {
    public:
        GaussianFilter(const PropertyList &propList) {

            radius_ = propList.GetFloat("radius", 2.0f);
            stddev_ = propList.GetFloat("stddev", 0.5f);
        }

        float Eval(float x) const override {
            float alpha = -1.0f / (2.0f * stddev_ * stddev_);
            return std::max(0.0f,
                            std::exp(alpha * x * x) -
                            std::exp(alpha * radius_ * radius_));
        }

    protected:
        float stddev_;
    };

    PHOENIX_REGISTER_CLASS(GaussianFilter, "gaussian");

    class MitchellNetravaliFilter : public Filter {
    public:
        MitchellNetravaliFilter(const PropertyList &propList) {
            /* Filter size in pixels */
            radius_ = propList.GetFloat("radius", 2.0f);
            /* B parameter from the paper */
            B_ = propList.GetFloat("B", 1.0f / 3.0f);
            /* C parameter from the paper */
            C_ = propList.GetFloat("C", 1.0f / 3.0f);
        }

        float Eval(float x) const override {
            x = std::abs(2.0f * x / radius_);
            float x2 = x * x, x3 = x2 * x;

            if (x < 1) {
                return 1.0f / 6.0f * ((12 - 9 * B_ - 6 * C_) * x3
                                      + (-18 + 12 * B_ + 6 * C_) * x2 + (6 - 2 * B_));
            } else if (x < 2) {
                return 1.0f / 6.0f * ((-B_ - 6 * C_) * x3 + (6 * B_ + 30 * C_) * x2
                                      + (-12 * B_ - 48 * C_) * x + (8 * B_ + 24 * C_));
            } else {
                return 0.0f;
            }
        }


    protected:
        float B_, C_;
    };

    PHOENIX_REGISTER_CLASS(MitchellNetravaliFilter,"MN");



class TentFilter : public Filter {
 public:
  TentFilter(const PropertyList &propList) {

    radius_ = propList.GetFloat("radius", 1.0f);
  }

  float Eval(float x) const override {
    return std::max((float ) 0.0f, 1.0f - std::abs(x / radius_));
  }

 protected:
};

PHOENIX_REGISTER_CLASS(TentFilter, "tent");

PHOENIX_NAMESPACE_END
