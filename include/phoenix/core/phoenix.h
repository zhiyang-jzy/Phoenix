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
#include<vector>
#include<unordered_map>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::unordered_map;

PHOENIX_NAMESPACE_BEGIN

class PhoenixObject;
class PropertyList;
class Camera;
class Sampler;
class Integrator;
class Shape;
class Ray;
class Interaction;
class Pembree;


typedef Eigen::Vector2f Point2f;
typedef Eigen::Vector3f Point3f;
typedef Eigen::Vector3f Vector3f;
typedef Eigen::Vector2f Vector2f;
typedef Eigen::Vector2i Vector2i;
typedef Eigen::Vector2i Point2i;
typedef Eigen::Affine3f Transform;



PHOENIX_NAMESPACE_END