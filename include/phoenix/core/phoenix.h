#pragma once



#define PHOENIX_NAMESPACE_BEGIN namespace phoenix {
#define PHOENIX_NAMESPACE_END }

#include<string>
#include<functional>
#include<map>
#include<unordered_map>
#include<memory>

using std::string;
using std::shared_ptr;
using std::make_shared;

PHOENIX_NAMESPACE_BEGIN

class PhonixObject;
class PropertyList;
class Camera;

class Spectrum;


PHOENIX_NAMESPACE_END