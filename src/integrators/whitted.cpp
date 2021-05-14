////
//// Created by jzy99 on 2021/1/25.
////
//
//#include<phoenix/core/integrator.h>
//
//PHOENIX_NAMESPACE_BEGIN
//
//class WhittedIntegrator: public Integrator
//{
//
// public:
//  WhittedIntegrator(const PropertyList& props)
//  {
//
//  }
//
//  Color3f Li(shared_ptr<Scene> scene,shared_ptr<Sampler> sampler,const Ray& ray) const override{
//    Interaction its;
//    if (!scene->Intersect(ray, its))
//      return Color3f(0.0f);
//
//    if (its.shape->GetBSDF()->IsDiffuse())
//    {
//
//      auto bsdf = its.shape->GetBSDF();
//
//      Color3f result(0.0f);
//
//      int randomIndex = rand() % scene->emitters_.size();
//      auto  emi = scene->emitters_.at(randomIndex);
//      EmitterQueryRecord iRec = EmitterQueryRecord(its.point);
//      auto emi2 = its.shape->GetEmitter();
//      if (emi2)
//      {
//        EmitterQueryRecord iRec2(ray.orig_, its.point, its.normal);
//        result = result + emi2->Eval(iRec2);
//      }
//
//      Color3f Lo = scene->emitters_.size() * emi->Sample(iRec, sampler->Next2D()); //Instead of iterating over all emitter, mulply because MC
//
//      Interaction its2;
//      bool hit_res = scene->Intersect(iRec.shadowRay, its2);
//
//      if (hit_res)
//        Lo = Color3f(0);
//
//      BSDFQueryRecord query = BSDFQueryRecord(its.geoFrame.ToLocal(iRec.wi),
//                                              its.geoFrame.ToLocal(-ray.dir_));
//
//      Color3f bsdfVal = bsdf->Eval(query);
//
//      float cos0 = std::abs(its.geoFrame.n.dot(iRec.wi));
//
//      result = result + Lo * bsdfVal * cos0;
//      return result;
//    }
//
//    else {
//      auto bsdf = its.shape->GetBSDF();
//      BSDFQueryRecord query = BSDFQueryRecord(its.geoFrame.ToLocal(-ray.dir_.normalized()));
//
//      bsdf->Sample(query, sampler->Next2D());
//      float _pdf = bsdf->Pdf(query);
//      Ray out(its.point, its.geoFrame.ToWorld(query.wo));
//      float rand = sampler->Next1D();
//
//      if (rand < 0.95) {
//        return Li(scene, sampler, out)/0.95;
//      }
//      else {
//        return { 0,0,0 };
//      }
//    }
//  }
//};
//
//PHOENIX_REGISTER_CLASS(WhittedIntegrator,"whitted");
//
//
//PHOENIX_NAMESPACE_END
