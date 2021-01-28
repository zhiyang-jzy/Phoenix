//
// Created by jzy99 on 2021/1/24.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_DPDF_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_DPDF_H_

#include<phoenix/core/phoenix.h>


PHOENIX_NAMESPACE_BEGIN

class DiscretePDF {
 public:
  explicit DiscretePDF(size_t nEntries = 0) {
    Reserve(nEntries);
    Clear();
  }
  void Clear() {
    cdf_.clear();
    cdf_.push_back(0.0f);
    normalized_ = false;
  }

    
  void Reserve(size_t nEntries) {
    cdf_.reserve(nEntries+1);
  }

    
  void Append(float pdf_value) {
    cdf_.push_back(cdf_[cdf_.size()-1] + pdf_value);
  }

    
  size_t Size() const {
    return cdf_.size()-1;
  }

    
  float operator[](size_t entry) const {
    return cdf_[entry+1] - cdf_[entry];
  }

    
  bool IsNormalized() const {
    return normalized_;
  }

  /**
   * \brief Return the original (unnormalized) sum of all PDF entries
   *
   * This assumes that \ref normalize() has previously been called
   */
  float GetSum() const {
    return sum_;
  }

  /**
   * \brief Return the normalization factor (i.e. the inverse of \ref getSum())
   *
   * This assumes that \ref normalize() has previously been called
   */
  float GetNormalization() const {
    return normalization_;
  }

  /**
   * \brief Normalize the distribution
   *
   * \return Sum of the (previously unnormalized) entries
   */
  float normalize() {
    sum_ = cdf_[cdf_.size()-1];
    if (sum_ > 0) {
      normalization_ = 1.0f / sum_;
      for (size_t i=1; i<cdf_.size(); ++i)
        cdf_[i] *= normalization_;
      cdf_[cdf_.size()-1] = 1.0f;
      normalized_ = true;
    } else {
      normalization_ = 0.0f;
    }
    return sum_;
  }

  /**
   * \brief %Transform a uniformly distributed sample to the stored distribution
   *
   * \param[in] sampleValue
   *     An uniformly distributed sample on [0,1]
   * \return
   *     The discrete index associated with the sample
   */
  [[nodiscard]] size_t Sample(float sample_value) const {
    auto entry =
        std::lower_bound(cdf_.begin(), cdf_.end(), sample_value);
    size_t index = (size_t) std::max((ptrdiff_t) 0, entry - cdf_.begin() - 1);
    return std::min(index, cdf_.size()-2);
  }

  /**
   * \brief %Transform a uniformly distributed sample to the stored distribution
   *
   * \param[in] sampleValue
   *     An uniformly distributed sample on [0,1]
   * \param[out] pdf
   *     Probability value of the sample
   * \return
   *     The discrete index associated with the sample
   */
  size_t Sample(float sampleValue, float &pdf) const {
    size_t index = Sample(sampleValue);
    pdf = operator[](index);
    return index;
  }

  /**
   * \brief %Transform a uniformly distributed sample to the stored distribution
   *
   * The original sample is value adjusted so that it can be "reused".
   *
   * \param[in, out] sampleValue
   *     An uniformly distributed sample on [0,1]
   * \return
   *     The discrete index associated with the sample
   */
  size_t sampleReuse(float &sampleValue) const {
    size_t index = Sample(sampleValue);
    sampleValue = (sampleValue - cdf_[index])
        / (cdf_[index + 1] - cdf_[index]);
    return index;
  }

  /**
   * \brief %Transform a uniformly distributed sample.
   *
   * The original sample is value adjusted so that it can be "reused".
   *
   * \param[in,out]
   *     An uniformly distributed sample on [0,1]
   * \param[out] pdf
   *     Probability value of the sample
   * \return
   *     The discrete index associated with the sample
   */
  size_t sampleReuse(float &sampleValue, float &pdf) const {
    size_t index = Sample(sampleValue, pdf);
    sampleValue = (sampleValue - cdf_[index])
        / (cdf_[index + 1] - cdf_[index]);
    return index;
  }

 private:
  std::vector<float> cdf_;
  float sum_, normalization_;
  bool normalized_;
};


PHOENIX_NAMESPACE_END



#endif //PHOENIX_INCLUDE_PHOENIX_CORE_DPDF_H_
