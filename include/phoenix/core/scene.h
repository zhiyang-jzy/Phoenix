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
    private:
        shared_ptr<Camera> m_camera;
        shared_ptr<Sampler> m_sampler;
        shared_ptr<Integrator> m_integrator;
        vector<shared_ptr<Shape> > m_shapes;
        shared_ptr<Pembree> m_embree;


    public:
    [[nodiscard]] PClassType getClassType()const override{return PClassType::PScene;}
    string toString()const override{return "scene";}

    bool castRay(const Ray& ray,Intersection& it)const
    {

    }

    void addChild(const shared_ptr<PhoenixObject> child)override;

    };


PHOENIX_NAMESPACE_END


#endif //PHOENIX_SCENE_H
