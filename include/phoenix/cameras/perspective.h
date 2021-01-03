#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/camera.h>

PHOENIX_NAMESPACE_BEGIN

class PerspectiveCamera: public Camera{
    PerspectiveCamera(const PropertyList& prop);

};

PHOENIX_REGISTER_CLASS(PerspectiveCamera,"perspecive");

PHOENIX_NAMESPACE_END