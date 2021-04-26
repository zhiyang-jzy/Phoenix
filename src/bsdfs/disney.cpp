//
// Created by 焦智洋 on 2021/4/20.
//

#include<phoenix/core/phoenix.h>
#include<phoenix/core/bsdf.h>
#include<phoenix/core/common.h>
#include<phoenix/core/sampling.h>

PHOENIX_NAMESPACE_BEGIN

    Color3f lerp(float t, Color3f v1, Color3f v2) {
        return (1 - t) * v1 + t * v2;
    }

    float SchlickFresnel(float x) {
        x = Clamp(1 - x, 0.f, 1.f);
        return pow(x, 5);
    }

    float SmithGGX(float cT, float a) {
        return 1 / (cT + sqrt(pow(cT, 2) + pow(a, 2) - pow(cT * a, 2)));
    }

    class DisneyBRDF : public BSDF {
    private:
        Color3f base_color_;
        bool cover_texture_;
        float subsurface_, metallic_, specular_, specular_tint_, roughness_, anisotropic_, sheen_tint, clearcoat_, clearcoat_gloss, sheen_;

    public:
        DisneyBRDF(const PropertyList &proplist) {
            cover_texture_ = proplist.HasVal("baseColor");
            base_color_ = proplist.GetColor("baseColor", Color3f(1, 1, 1));
            subsurface_ = proplist.GetFloat("subsurface", 0.5);
            metallic_ = proplist.GetFloat("metallic", 0.5);
            specular_ = proplist.GetFloat("specular", 0.5);
            specular_tint_ = proplist.GetFloat("specularTint", 0.5);
            roughness_ = proplist.GetFloat("roughness", 0.5);
            anisotropic_ = proplist.GetFloat("anisotropic", 0.5);
            sheen_ = proplist.GetFloat("sheen", 0.5);
            sheen_tint = proplist.GetFloat("sheenTint", 0.5);
            clearcoat_ = proplist.GetFloat("clearcoat", 0.5);
            clearcoat_gloss = proplist.GetFloat("clearcoatGloss", 0.5);
        }

      Color3f GetAlbedo(Color3f texcolor) const override{
          if(cover_texture_)
            return base_color_;
        return texcolor;
      }
        Color3f Eval(const BSDFQueryRecord &bRec, Color3f albedo) const override {
            if (bRec.wi.z() < 0 || bRec.wo.z() < 0)
                return Color3f(0.0f);
            Vector3f l = bRec.wo, v = bRec.wi;
            Vector3f h = (l + v).normalized();
            float cos_l = l.z(), cos_v = v.z();

            Color3f Cdlin = cover_texture_? base_color_.pow(2.2):albedo.pow(2.2);
            float Cdlum = .3 * Cdlin.x() + .6 * Cdlin.y() + .1 * Cdlin.z();

            Color3f Ctint = (Cdlum > 0) ? Color3f(Cdlin.x() / Cdlum, Cdlin.y() / Cdlum, Cdlin.z() / Cdlum) : Color3f(1,
                                                                                                                     1,
                                                                                                                     1);
            Color3f Ctintmix = .08 * specular_ * lerp(specular_tint_, Color3f(1, 1, 1), Ctint);
            Color3f Cspec = lerp(metallic_, Ctintmix, Cdlin);

            float FD90 = 0.5 + 2 * pow(l.dot(h), 2) * roughness_;
            float FL = SchlickFresnel(cos_l);
            float FV = SchlickFresnel(cos_v);
            float fd = lerp(FL, 1, FD90) * lerp(FV, FD90, 1);
            // Hanrahan-Krueger subsurface BRDF approximation
            float Fss90 = pow(l.dot(h), 2) * roughness_;
            float Fss = lerp(FL, 1.0, Fss90) * lerp(FV, 1.0, Fss90);
            float ss = 1.25 * (Fss * (1 / (cos_l + cos_v) - .5) + .5);

            // ---------- Specular
            float alpha = std::max(0.01, pow(roughness_, 2));
            float Ds = SquareToGTR2Pdf(h, alpha);
            float FH = SchlickFresnel(l.dot(h));
            Color3f Fs = lerp(FH, Cspec, Color3f(1,1,1));
            float Gs = SmithGGX(cos_l, alpha) * SmithGGX(cos_v, alpha);

            // ---------- Sheen
            Color3f Csheen = lerp(sheen_tint, Color3f(1, 1, 1), Ctint);
            Color3f Fsheen = FH * sheen_ * Csheen;

            // ---------- Clearcoat
            float Dr = SquareToGTR1Pdf(h, lerp(clearcoat_gloss, .1, .001));
            float Fr = lerp(FH, .04, 1);
            float Gr = SmithGGX(cos_l, .25) * SmithGGX(cos_v, .25);


            return (1 - metallic_) * (INV_PI * lerp(subsurface_, fd, ss) * Cdlin + Fsheen)
                   + Gs * Fs * Ds  + .25 * clearcoat_ * Gr * Fr * Dr;
        }

        Color3f Sample(BSDFQueryRecord &bRec,const Point2f& sample,Color3f albedo)const override{
            if (bRec.wi.z() <= 0)
                return Color3f(0.0f);

            float diffuseR = (1 - metallic_) / 2;

            if (sample.x() < diffuseR) {
                Point2f sample1(sample.x() / diffuseR, sample.y());
                bRec.wo = SquareToCosineHemisphere(sample1);
            } else {
                Point2f sample1((sample.x() - diffuseR) / (1 - diffuseR), sample.y());
                float GTR2R = 1 / (1 + clearcoat_);

                if (sample1.x() < GTR2R) {
                    Point2f sample2(sample1.x() / GTR2R, sample1.y());
                    float alpha = std::max(0.01, pow(roughness_, 2));
                    Vector3f h = SquareToGTR2(sample2, alpha);
                    bRec.wo = ((2.f * h.dot(bRec.wi) * h) - bRec.wi).normalized();
                } else {
                    Point2f sample2((sample1.x() - GTR2R) / (1 - GTR2R), sample1.y());
                    Vector3f h = SquareToGTR1(sample2, lerp(clearcoat_gloss, .1, .001));
                    bRec.wo = ((2.f * h.dot(bRec.wi) * h) - bRec.wi).normalized();
                }
            }

            if (bRec.wo.z() <= 0)
                return Color3f(0.0f);

            return Eval(bRec,albedo) / Pdf(bRec) * bRec.wo.z();
        }

        float Pdf(const BSDFQueryRecord& bRec)const override{
            Vector3f h = (bRec.wi + bRec.wo).normalized();
            if (bRec.wo.z() <= 0)
                return 0.0f;

            float GTR2R = 1 / (1 + clearcoat_);
            float diffuseR = (1 - metallic_) / 2;

            float alpha = std::max(0.01, pow(roughness_, 2));

            return diffuseR * bRec.wo.z() * INV_PI
                   + (1 - diffuseR) * ((GTR2R) * SquareToGTR2Pdf(h, alpha) + (1 - GTR2R) * SquareToGTR1Pdf(h, lerp(clearcoat_gloss, .1, .001)));
        }
    };

    PHOENIX_REGISTER_CLASS(DisneyBRDF,"disney");

PHOENIX_NAMESPACE_END
