   
   
   

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

class PathmisIntegrator : public Integrator
{
 public:
  PathmisIntegrator(const PropertyList& props){}
  Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
    Interaction its;

       
    if (!scene->Intersect(ray, its))
      return Color3f(0.0f);
       

    Color3f Le = Color3f(0.0f);
    auto emi2 = its.shape->GetEmitter();
    if (emi2) {
         
      EmitterQueryRecord iRec2(ray.orig_, its.point, its.normal);
      Le = emi2->Eval(iRec2);
    }

    auto bsdf = its.shape->GetBSDF();
       


    int rand_index = rand() % scene->emitters_.size();

    auto emmit = scene->emitters_.at(rand_index);

    EmitterQueryRecord iRec(its.point);

    Color3f Ld = scene->emitters_.size() * emmit->Sample(iRec, sampler->Next2D());


    Interaction its2;
    bool hit_res = scene->Intersect(iRec.shadowRay, its2);

    if (hit_res )
      Ld = Color3f(0);

    float cos0 = std::abs(its.geoFrame.n.dot(iRec.wi));


    BSDFQueryRecord query1 = BSDFQueryRecord(its.geoFrame.ToLocal(iRec.wi),
                                             its.geoFrame.ToLocal(-ray.dir_));

    float pdfa1 = emmit->Pdf(iRec);
    float pdfa2 = bsdf->Pdf(query1);

       


    Color3f bsdfVal1 = bsdf->Eval(query1);

    Ld = Ld * bsdfVal1 * cos0;



       

    Vector3f toCam = -ray.dir_.normalized();


    BSDFQueryRecord query(its.geoFrame.ToLocal(toCam));    
       
    Color3f bsdfVal = bsdf->Sample(query, sampler->Next2D());
    Ray lightRay(its.point, its.geoFrame.ToWorld(query.wo));


    float pdfb1 = bsdf->Pdf(query);
    float pdfb2 = 0;

       

    Interaction temp;




       


    if (scene->Intersect(lightRay, temp))
    {
      if (temp.shape->IsEmitter())
      {
        EmitterQueryRecord eqr(temp.point);

        temp.shape->GetEmitter()->Sample(eqr, sampler->Next2D());
        pdfb2 = eqr.pdf;
           
      }
    }

       

    float w1, w2;

    w1 = pdfa1 + pdfa2 >= EPSILON ? pdfa1 /(pdfa1 + pdfa2) : .0f;
    w2 = pdfb1 + pdfb2 >= EPSILON ? pdfb1 /(pdfb1 + pdfb2) : .0f;

    if (w1 < .0f || w1>1.f) {
      w1 = .0f;
    }
    if (w2 < .0f || w2>1.f) {
      w2 = .0f;
    }

    Color3f res;

    if (sampler->Next1D() > 0.05f)
      res = Le + w1 * Ld +w2 * this->Li(scene, sampler, lightRay) * bsdfVal / (1.f - 0.05f);
         

    else
      res = Le;
    return res;
  }

};

PHOENIX_REGISTER_CLASS(PathmisIntegrator,"pathmis");

PHOENIX_NAMESPACE_END