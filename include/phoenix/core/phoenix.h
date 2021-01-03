#pragma once



#define PHOENIX_NAMESPACE_BEGIN namespace phoenix {
#define PHOENIX_NAMESPACE_END }

#include<string>
#include<functional>
#include<map>
#include<unordered_map>
#include<memory>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using std::string;
using std::shared_ptr;
using std::make_shared;

PHOENIX_NAMESPACE_BEGIN

class PhonixObject;
class PropertyList;
class Camera;
class Spectrum;


typedef Eigen::Vector2f Point2f;
typedef Eigen::Vector3f Point3f;
typedef Eigen::Vector3f Vector3f;
typedef Eigen::Vector2f Vector2f;
typedef Eigen::Vector2i Vector2i;
typedef Eigen::Vector2i Point2i;
typedef Eigen::Matrix4f Transform;



PHOENIX_NAMESPACE_END