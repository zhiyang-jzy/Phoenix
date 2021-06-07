//
// Created by jzy99 on 2021/5/16.
//
#include<phoenix/core/bsdf.h>
#include<phoenix/core/frame.h>

PHOENIX_NAMESPACE_BEGIN

class RoughPlastic : public BSDF{
 public:
  RoughPlastic(const PropertyList& props)
  {

  }

  Color3f Sample(BSDFQueryRecord& bRec,const Point2f &sample,Color3f albedo)const override
  {
    float cos_theta = Frame::CosTheta(bRec.wi);

    if(cos_theta<=0.0f)
      return Color3f(0.0f);


  }




 private:
  float intIOR_,extIOR_,alpha_;

};



PHOENIX_NAMESPACE_END
