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

class PhotonMapper : public Integrator {

  typedef PointKDTree<Photon> PhotonMap;

 public:

  PhotonMapper(const PropertyList &props) {
    photon_count_ = props.GetInteger("photonCount", 1000);
    photon_radius_ = props.GetFloat("photonRadius", 0.05f);
    rr_start_ = props.GetInteger("rrStart", 5);
    max_depth_ = props.GetInteger("maxDepth", -1);
  }

  void Preprocess(const shared_ptr<Scene> scene) override {
    spdlog::info("gathering photon");
    auto sampler =
        std::dynamic_pointer_cast<Sampler>(PhoenixObjectFactory::CreateInstance("independent", PropertyList()));

    photon_map_ = make_shared<PhotonMap>();
    photon_map_->reserve(photon_count_);

    int stored_photons = 0,emitted_photons=0;

    while (stored_photons < photon_count_)
    {
      // First choose a light
//      std::cerr <<"\rtest"<< (float)stored_photons/(float)photon_count_ << ' ' << std::flush;
      float pdf;
      auto emitter = scene->GetRandomEmitter(sampler,pdf);

      if (emitter->GetEmitterType() == Emitter::EmitterType::AREA)
      {
        Ray photon_ray;
        Color3f photon_power = emitter->SamplePhoton(photon_ray,sampler);
        if (!photon_power.isZero())
        {
          emitted_photons++;			// keep track of how many photons we shot to divide the contrib of all stored photons finally

          Interaction isect;
          int depth = 0;
          while ((depth < max_depth_ || max_depth_ == -1) && stored_photons < photon_count_)
          {

            if (!scene->Intersect(photon_ray, isect))
            {
              break;
            }
            auto bsdf = isect.shape->GetBSDF();

            if (bsdf->IsDiffuse())
            {
              // Store photon
              photon_map_->push_back(Photon(isect.point, photon_ray.dir_, photon_power));
              stored_photons++;
            }

            // Now sample next direction
            BSDFQueryRecord bRec(isect.geoFrame.ToLocal(-photon_ray.dir_));
            Color3f f = bsdf->Sample(bRec, sampler->Next2D(),isect.albedo);
            Vector3f reflected_dir = isect.geoFrame.ToWorld(bRec.wo);

            Color3f incoming_power = photon_power;
            photon_power *= f * fabsf(Frame::CosTheta(bRec.wo));
            if (f.isZero())
              break;
            if (depth > rr_start_)
            {
              float p = 1.0f - photon_power.getLuminance() / incoming_power.getLuminance();
              if (sampler->Next1D() < p)
                break;
              else photon_power /= (1.0f - p);
            }
            if (depth > max_depth_ && max_depth_ != -1)
            {
              break;
            }
            photon_ray = Ray(isect.point, reflected_dir);
            depth++;
          }
        }
      }
    }


    photon_map_->scale(stored_photons);
    photon_map_->build();

  }

  Color3f Li(const shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
    Color3f L(0.0f);
    Color3f throughput(1.0f);
    int depth = 0;
    Interaction isect;
    bool bLastBounceWasSpecular = false;
    Ray traced_ray = ray;
    while (depth < max_depth_ || max_depth_ == -1)
    {
      if (!scene->Intersect(traced_ray, isect))
        return Color3f(0,0,0);
      //TODO

      if (isect.shape->IsEmitter())
      {
        EmitterQueryRecord eRec;
        eRec.ref = traced_ray.orig_;
        eRec.wi = traced_ray.dir_;
        eRec.n = isect.geoFrame.n;
        L += throughput * isect.shape->GetEmitter()->Eval(eRec);

        break;
      }

      auto bsdf = isect.shape->GetBSDF();

      // Compute indirect contribution only from diffuse surfaces
      if (bsdf->IsDiffuse())
      {
        std::vector<uint32_t> results;
        photon_map_->search(isect.point, photon_radius_, results);
        float area = PI * photon_radius_*photon_radius_;

        // The uint32_t makes the size() - 1 wrap around. Subtle bug.
        if (results.size() > 0)
        {
          for (uint32_t i = 0; i < results.size() - 1; i++)
          {
            const Photon &photon = (*photon_map_)[results[i]];

            // Compute the integral equation
            BSDFQueryRecord bRec(isect.geoFrame.ToLocal(-traced_ray.dir_), isect.geoFrame.ToLocal(-photon.getDirection()));

            L += throughput * bsdf->Eval(bRec,isect.albedo) * photon.getPower() / area;
          }
        }
        break;
      }

      // Sample a reflection ray
      BSDFQueryRecord bRec(isect.geoFrame.ToLocal(-traced_ray.dir_));
      Color3f f = bsdf->Sample(bRec, sampler->Next2D(),isect.albedo);
      Vector3f reflected_dir = isect.geoFrame.ToWorld(bRec.wo);

      float cos_theta = fabsf(Frame::CosTheta(bRec.wo));
      throughput *= f * cos_theta;

      // Check if we've fa
      if (throughput.isZero())
        break;

      // Check for russian roulette
      if (depth > rr_start_)
      {
        if (sampler->Next1D() < 0.99f)
          break;
        else throughput /= 0.99f;
      }
      else if (depth > max_depth_ && max_depth_ != -1)
      {
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

  shared_ptr<PhotonMap> photon_map_;

};

PHOENIX_REGISTER_CLASS(PhotonMapper, "PhotonMapper");

PHOENIX_NAMESPACE_END
