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
 protected:
  pcg32 random_;
 public:
  Independent(const PropertyList& props){

 }
  float Next1D()override{
    return random_.nextFloat();
  }

  Point2f Next2D()override{
    return {random_.nextFloat(),random_.nextFloat()};
  }
};

PHOENIX_REGISTER_CLASS(Independent,"independent");




PHOENIX_NAMESPACE_END

