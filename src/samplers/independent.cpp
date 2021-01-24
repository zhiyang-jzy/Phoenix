//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/phoenix.h>
#include<phoenix/core/sampler.h>
#include<phoenix/core/properlist.h>
#include<pcg/pcg32.h>

PHOENIX_NAMESPACE_BEGIN

class Independent : public Sampler
{
 public:
  pcg32 random_;

 public:
  explicit Independent(const PropertyList& props){
    sample_count_ = props.GetInteger("sampleCount",1);
 }
  float Next1D()override{
    return random_.nextFloat();
  }
  void Prepare(const ImageBlock& block)override{
    random_.seed(
        block.GetOffset().x(),
        block.GetOffset().y()
    );
  }
  Independent()= default;

  [[nodiscard]] shared_ptr<Sampler> Clone()const override{
    auto cloned = make_shared<Independent>();
    cloned->sample_count_ = sample_count_;
    cloned->random_ = random_;
    return std::move(cloned);
  }
  Point2f Next2D()override{
    return {random_.nextFloat(),random_.nextFloat()};
  }
};

PHOENIX_REGISTER_CLASS(Independent,"independent");




PHOENIX_NAMESPACE_END

