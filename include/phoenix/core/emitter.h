//
// Created by jzy99 on 2021/1/3.
//

#ifndef PHOENIX_EMITTER_H
#define PHOENIX_EMITTER_H


#include <phoenix/core/object.h>

PHOENIX_NAMESPACE_BEGIN

    class Emitter : public PhoenixObject {

    PClassType getClassType()const override{return PClassType::PEmitter;}

    string toString()const override{return "emitter"};

    };

PHOENIX_NAMESPACE_END

#endif //PHOENIX_EMITTER_H
