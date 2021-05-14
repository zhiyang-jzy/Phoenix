//
// Created by jzy99 on 2021/5/14.
//

//
// Created by jzy99 on 2021/5/13.
//
#include<phoenix/core/integrator.h>
#include<phoenix/core/sampler.h>
#include<phoenix/core/photon.h>
#include<thread>
#include<tbb/tbb.h>
#include<algorithm>
PHOENIX_NAMESPACE_BEGIN

class PhotonMapper1 : public Integrator {

  typedef PointKDTree<Photon> PhotonMap;

 public:

  PhotonMapper1(const PropertyList &props) {
    photon_count_ = props.GetInteger("photonCount", 1000);
    photon_radius_ = props.GetFloat("photonRadius", 0.05f);
    rr_start_ = props.GetInteger("rrStart", 5);
    max_depth_ = props.GetInteger("maxDepth", -1);
  }

  void TraceGlobalPhoton(const shared_ptr<Scene> &scene,
                         shared_ptr<Sampler> &sampler) {
    global_photon_ = make_shared<PhotonMap>();
    global_photon_->reserve(photon_count_);

    int stored_photons = 0, emitted_photons = 0;
    while (stored_photons < photon_count_) {
//      std::cerr << "\rtest" << (float) stored_photons / (float) photon_count_ << ' ' << std::flush;
      float pdf;
      auto emitter = scene->GetRandomEmitter(sampler, pdf);
      if (emitter->GetEmitterType() == Emitter::EmitterType::AREA) {
        Ray photon_ray;
        Color3f photon_power = emitter->SamplePhoton(photon_ray, sampler);
        if (!photon_power.isZero()) {
          emitted_photons++;
          Interaction inter;
          int depth = 0;
          while ((depth < max_depth_ || max_depth_ == -1) && stored_photons < photon_count_) {
            if (!scene->Intersect(photon_ray, inter)) {
              break;
            }
            auto bsdf = inter.shape->GetBSDF();
            if (bsdf->IsDiffuse()) {
              global_photon_->push_back(Photon(inter.point, photon_ray.dir_, photon_power));
              stored_photons++;
            }
            BSDFQueryRecord bRec(inter.geoFrame.ToLocal(-photon_ray.dir_));
            Color3f f = bsdf->Sample(bRec, sampler->Next2D(), inter.albedo);
            Vector3f reflected_dir = inter.geoFrame.ToWorld(bRec.wo);
            Color3f incoming_power = photon_power;
            photon_power *= f * fmax(0.0f,fabsf(Frame::CosTheta(bRec.wo)));
            if (f.isZero())
              break;
            if (depth > rr_start_) {
              float p = 1.0f - photon_power.getLuminance() / incoming_power.getLuminance();
              if (sampler->Next1D() < p)
                break;
              else photon_power /= (1.0f - p);
            }
            if (depth > max_depth_ && max_depth_ != -1) {
              break;
            }
            photon_ray = Ray(inter.point, reflected_dir);
            depth++;
          }
        }
      }
    }
    global_photon_->scale(emitted_photons);
    global_photon_->build();
  }
  void TraceCausticPhoton(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler) {
    caustic_photon_ = make_shared<PhotonMap>();
    caustic_photon_->reserve(photon_count_);
    int stored_photons = 0, emitted_photons = 0;
    while (stored_photons < photon_count_) {
      float pdf;
      auto emitter = scene->GetRandomEmitter(sampler, pdf);
      Ray photon_ray;
      Color3f photon_power = emitter->SamplePhoton(photon_ray, sampler);
      if (!photon_power.isZero()) {
        emitted_photons++;
        Interaction inter;
        int depth = 0;
        bool has_glossy = false;
        while ((depth < max_depth_ || max_depth_ == -1) && stored_photons < photon_count_) {
          if (!scene->Intersect(photon_ray, inter)) {
            break;
          }
          auto bsdf = inter.shape->GetBSDF();
          if (bsdf->IsDiffuse()) {
            if (has_glossy) {
              caustic_photon_->push_back(Photon(inter.point, photon_ray.dir_, photon_power));
              stored_photons++;
            }
            break;
          } else {
            has_glossy = true;
          }
          BSDFQueryRecord bRec(inter.geoFrame.ToLocal(-photon_ray.dir_));
          Color3f f = bsdf->Sample(bRec, sampler->Next2D(), inter.albedo);
          Vector3f reflected_dir = inter.geoFrame.ToWorld(bRec.wo);
          Color3f incoming_power = photon_power;
          photon_power *= f * fabsf(Frame::CosTheta(bRec.wo));
          if (f.isZero())
            break;
          if (depth > rr_start_) {
            float p = 1.0f - photon_power.getLuminance() / incoming_power.getLuminance();
            if (sampler->Next1D() < p)
              break;
            else photon_power /= (1.0f - p);
          }
          if (depth > max_depth_ && max_depth_ != -1) {
            break;
          }
          photon_ray = Ray(inter.point, reflected_dir);
          depth++;
        }
      }
    }
    caustic_photon_->scale(emitted_photons);
    caustic_photon_->build();
  }

  void Preprocess(const shared_ptr<Scene> scene) override {
    spdlog::info("gathering photon");
    auto sampler =
        std::dynamic_pointer_cast<Sampler>(PhoenixObjectFactory::CreateInstance("independent", PropertyList()));
    TraceGlobalPhoton(scene, sampler);
    TraceCausticPhoton(scene, sampler);

  }

  Color3f Li(const shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
    Color3f L(0.0f);
    Color3f throughput(1.0f);
    int depth = 0;
    Interaction isect;
    Ray traced_ray = ray;
    while (depth < max_depth_ || max_depth_ == -1) {
      if (!scene->Intersect(traced_ray, isect))
        return Color3f(0, 0, 0);
      //TODO

      if (isect.shape->IsEmitter()) {
        EmitterQueryRecord eRec;
        eRec.ref = traced_ray.orig_;
        eRec.wi = traced_ray.dir_;
        eRec.n = isect.geoFrame.n;
        L += throughput * isect.shape->GetEmitter()->Eval(eRec);
        break;
      }

      {
        float emitter_pdf;
        auto _emitter = scene->GetRandomEmitter(sampler, emitter_pdf);

        EmitterQueryRecord ems(isect.point);
        Color3f Li_ems = _emitter->Sample(ems, sampler->Next2D());

        BSDFQueryRecord bRec_ems(isect.geoFrame.ToLocal(-ray.dir_), isect.geoFrame.ToLocal(ems.wi));
        bRec_ems.uv = isect.uv;

        Color3f L_ems(0);

        Color3f f_ems = isect.shape->GetBSDF()->Eval(bRec_ems, isect.albedo);
        Interaction its_ems;
        if (!scene->Intersect(ems.shadowRay, its_ems))
          L_ems = throughput*f_ems * Li_ems  * scene->GetEmiitersCount();

        L += L_ems;

      }

      auto bsdf = isect.shape->GetBSDF();

      BSDFQueryRecord bRec_(isect.geoFrame.ToLocal(-traced_ray.dir_));
      Color3f f = bsdf->Sample(bRec_, sampler->Next2D(), isect.albedo);

      // Compute indirect contribution only from diffuse surfaces
      if (bsdf->IsDiffuse()) {
        {
          std::vector<uint32_t> results;
          caustic_photon_->search(isect.point, photon_radius_, results);
          float area = PI * photon_radius_ * photon_radius_;
          // The uint32_t makes the size() - 1 wrap around. Subtle bug.
          if (results.size() > 0) {
            for (uint32_t i = 0; i < results.size() - 1; i++) {
              const Photon &photon = (*caustic_photon_)[results[i]];

              // Compute the integral equation
              BSDFQueryRecord
                  bRec(isect.geoFrame.ToLocal(-traced_ray.dir_), isect.geoFrame.ToLocal(-photon.getDirection()));

              L += throughput * bsdf->Eval(bRec, isect.albedo) * photon.getPower() / area;
            }
          }

          {
            BSDFQueryRecord rec(isect.geoFrame.ToLocal(-ray.dir_).normalized());
            Color3f bsdfValue = bsdf->Sample(rec, sampler->Next2D(), isect.albedo);
            Ray rec_ray(isect.point, isect.geoFrame.ToWorld(rec.wo));

            Interaction rec_inter;
            if (scene->Intersect(rec_ray, rec_inter)) {
              auto rec_bsdf = rec_inter.shape->GetBSDF();
              if (rec_bsdf->IsDiffuse()) {
                Color3f rec_res(0);
                results.clear();
                global_photon_->search(rec_inter.point, photon_radius_, results);
                if (results.size() > 0) {
                  for (uint32_t i = 0; i < results.size() - 1; i++) {
                    const Photon &photon = (*global_photon_)[results[i]];

                    // Compute the integral equation
                    BSDFQueryRecord
                        rec_rec
                        (rec_inter.geoFrame.ToLocal(-rec_ray.dir_), rec_inter.geoFrame.ToLocal(-photon.getDirection()));
                    rec_res = throughput * rec_bsdf->Eval(rec_rec, rec_inter.albedo) * photon.getPower() / area;
                    L += rec_res*f;
                  }
                }
              }
            }
          }
        }
        // Immediately stop recursion
        break;
      }

      // Sample a reflection ray

      Vector3f reflected_dir = isect.geoFrame.ToWorld(bRec_.wo);


      throughput *= f;

      // Check if we've fa
      if (throughput.isZero())
        break;

      // Check for russian roulette
      if (depth > rr_start_) {
        if (sampler->Next1D() < 0.5f)
          break;
        else throughput *= 2.0f;
      } else if (depth > max_depth_ && max_depth_ != -1) {
        // forcibly terminate
        break;
      }
      // Propogate
      traced_ray = Ray(isect.point, reflected_dir);
      depth++;
    }
    return L;
  }

 private:
  int photon_count_, rr_start_, max_depth_;
  float photon_radius_;

  shared_ptr<PhotonMap> global_photon_, caustic_photon_;

};

PHOENIX_REGISTER_CLASS(PhotonMapper1, "PhotonMapper1");

PHOENIX_NAMESPACE_END
