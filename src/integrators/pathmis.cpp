

#include<phoenix/core/integrator.h>

PHOENIX_NAMESPACE_BEGIN

    class PathmisIntegrator : public Integrator {
    public:
        PathmisIntegrator(const PropertyList &props) {}

        Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
            Color3f Li = 0, t = 1;
            Ray rayR = ray;
            float prob = 1, w_mats = 1, w_ems = 1;
            Color3f f(1, 1, 1);

            while (true) {
                Interaction its;

                if (!scene->Intersect(rayR, its)) {
                    if (scene->GetEnvEmitter() == nullptr) {
                        return Li;
                    } else {
                        EmitterQueryRecord lRec;
                        lRec.wi = rayR.dir_;
                        return Li + w_mats * t * scene->GetEnvEmitter()->Eval(lRec);
                    }
                }

                // -------------------------------------Emitted--------------------------------------- //
                Color3f Le = 0;
                if (its.shape->IsEmitter()) {
                    EmitterQueryRecord lRecE(rayR.orig_, its.point, its.geoFrame.n);
                    Le = its.shape->GetEmitter()->Eval(lRecE);
                    Le = Color3f(1);
                }
                Li += t * w_mats * Le;
//                Li=Le;

                // -------------------------------------Russian roulette--------------------------------------- //
                prob = std::min(t.maxCoeff(), .99f);
                if (sampler->Next1D() >= prob) {
                    return Li;
                }

                t /= prob;

                // -------------------------------------EMS--------------------------------------- //
                Color3f L_ems = 0;

                // get a random emitter
                float emiter_pdf;
                const auto emitter = scene->GetRandomEmitter(sampler, emiter_pdf);

                // reflected
                EmitterQueryRecord lRec_ems;
                lRec_ems.ref = its.point;
                Color3f Li_ems = emitter->Sample(lRec_ems, sampler->Next2D()) * scene->GetEmitters().size();
                float pdf_ems = emitter->Pdf(lRec_ems);

                // BSDF
                BSDFQueryRecord bRec_ems(its.geoFrame.ToLocal(-rayR.dir_), its.geoFrame.ToLocal(lRec_ems.wi));
                bRec_ems.uv = its.uv;
                Color3f f_ems = its.shape->GetBSDF()->Eval(bRec_ems, its.albedo);
                if (abs(pdf_ems + its.shape->GetBSDF()->Pdf(bRec_ems)) > EPSILON)
                    w_ems = pdf_ems / (pdf_ems + its.shape->GetBSDF()->Pdf(bRec_ems));

                // check if shadow ray is occluded
                Interaction its_ems;
                if (!scene->Intersect(lRec_ems.shadowRay, its_ems))
                    L_ems = f_ems * Li_ems * std::max(0.f, its.geoFrame.ToLocal(lRec_ems.wi).z());

                Li += t * w_ems *L_ems;
//                Li=Li.abs();
//                    Li=Color3f(1);

                // -------------------------------------BSDF sampling--------------------------------------- //
                BSDFQueryRecord bRec(its.geoFrame.ToLocal(-rayR.dir_));
                Color3f f = its.shape->GetBSDF()->Sample(bRec, sampler->Next2D(), its.albedo);
                t *= f;
                // shoot next ray
                rayR = Ray(its.point, its.geoFrame.ToWorld(bRec.wo));

                // ----------------------------------MATS weight for next Le----------------------------------- //
                float pdf_mats = its.shape->GetBSDF()->Pdf(bRec);

                Interaction itsR;
                if (scene->Intersect(rayR, itsR)) {
                    if (itsR.shape->IsEmitter()) {
                        EmitterQueryRecord lRec_mats = EmitterQueryRecord(its.point, itsR.point, itsR.geoFrame.n);
                        if ((pdf_mats + itsR.shape->GetEmitter()->Pdf(lRec_mats)) > EPSILON)
                            w_mats = pdf_mats / (pdf_mats + itsR.shape->GetEmitter()->Pdf(lRec_mats));
                    }
                } else if (scene->GetEnvEmitter() != nullptr) {
                    EmitterQueryRecord lRec_mats;
                    lRec_mats.wi = rayR.dir_;
                    if (abs(pdf_mats + scene->GetEnvEmitter()->Pdf(lRec_mats)) > EPSILON)
                        w_mats = pdf_mats / (pdf_mats + scene->GetEnvEmitter()->Pdf(lRec_mats));
                }

            }
        }
    };

    PHOENIX_REGISTER_CLASS(PathmisIntegrator, "pathmis");

PHOENIX_NAMESPACE_END