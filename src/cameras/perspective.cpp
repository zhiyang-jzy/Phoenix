//
// Created by jzy99 on 2021/1/2.
//

#include<phoenix/cameras/perspective.h>
#include<phoenix/core/properlist.h>

PHOENIX_NAMESPACE_BEGIN

PerspectiveCamera::PerspectiveCamera(const PropertyList &prop) {

    m_outputSize.x() = prop.getInteger("width");
    m_outputSize.y() = prop.getInteger("height");


}



PHOENIX_NAMESPACE_END