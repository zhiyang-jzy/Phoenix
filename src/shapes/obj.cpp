//
// Created by jzy99 on 2021/1/19.
//

#include<phoenix/core/phoenix.h>
#include<phoenix/core/shape.h>
#include<phoenix/core/properlist.h>
#include<phoenix/core/model.h>
#include<phoenix/core/pembree.h>

PHOENIX_NAMESPACE_BEGIN

    class OBJ : public Shape {
    protected:
        Model model;
        std::map<unsigned int, shared_ptr<Mesh> > dict_;

    private:
        void ApplyTransform(Transform transform) {
            model.ApplyTransform(transform);
        }

    public:
        explicit OBJ(const PropertyList &props) {
            string filename = props.GetString("filename");
            spdlog::info("loading obj file: {}", filename);
            model.Load(filename);
            Transform transform = props.GetTransform("toWorld", Transform());
            ApplyTransform(transform);
            spdlog::info("area_: {}", model.area_);
            area_ = model.area_;
            inv_area_ = 1.0f / area_;
        }

        vector<unsigned int> AddToEmbree(Pembree &embree) override {
            vector<unsigned int> res;
            for (auto &mesh : model.meshes_) {

                std::vector<Eigen::Vector3f> temp;
                for (auto &vertex : mesh->vertices) {
                    temp.push_back(vertex.position);
                }
//	    std::cout<<"add "<<meshe.vertices.size()<<std::endl;
                unsigned int id = embree.AddMesh(temp, mesh->indices);
                dict_[id] = mesh;
                res.push_back(id);
            }
            return res;
        }

        SampleData SampleSurface(const Point2f &sample) const override {
            return model.SampleSurface(sample);
        }

        float PdfSurface(const SampleData &sRec) const override {
            return model.dpdf_.GetNormalization();
        }

        Color3f GetTextureColor(unsigned int geoid, unsigned int priid, Point2f uv) const override {
            const shared_ptr<Mesh> mesh = dict_.at(geoid);

            if (mesh->texture_ == nullptr) {
                return Color3f(0.3, 0.4, 0.5);
            }
            Point2f now_uv = mesh->getTexcood(priid, uv.x(), uv.y());
            return mesh->texture_->GetColor(now_uv);
        }

        Normal3f GetNormal(unsigned int geoid, unsigned int priid, Point2f uv, const Normal3f normal) const override {
            const shared_ptr<Mesh> mesh = dict_.at(geoid);
            Normal3f now_norm = mesh->GetNormal(priid, uv.x(), uv.y());
            return now_norm.normalized();
        }


    };

    PHOENIX_REGISTER_CLASS(OBJ, "obj");

PHOENIX_NAMESPACE_END