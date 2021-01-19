//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_SCENE_H
#define PHOENIX_SCENE_H


#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>
#include<phoenix/core/shape.h>


PHOENIX_NAMESPACE_BEGIN

    class Scene : public PhoenixObject {
    public:
        shared_ptr<Camera> camera_;
        shared_ptr<Sampler> sampler_;
        shared_ptr<Integrator> integrator_;
        vector<shared_ptr<Shape> > shapes_;
        shared_ptr<Pembree> embree_;


    public:
    [[nodiscard]] PClassType GetClassType()const override{return PClassType::PScene;}
    string ToString()const override{return "scene";}

    Scene(const PropertyList& props);

    bool CastRay(const Ray& ray,Interaction& it)const
    {

    }

    void AddChild(shared_ptr<PhoenixObject> child)override;

    };


PHOENIX_NAMESPACE_END


#endif //PHOENIX_SCENE_H
