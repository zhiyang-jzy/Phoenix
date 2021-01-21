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
struct Interaction;
struct Transform;
class Pembree;


template <typename Scalar, int Dimension>  struct TVector;
template <typename Scalar, int Dimension>  struct TPoint;
template <typename Point, typename Vector> struct TRay;
template <typename Point>                  struct TBoundingBox;

typedef TVector<float, 1>       Vector1f;
typedef TVector<float, 2>       Vector2f;
typedef TVector<float, 3>       Vector3f;
typedef TVector<float, 4>       Vector4f;
typedef TVector<double, 1>      Vector1d;
typedef TVector<double, 2>      Vector2d;
typedef TVector<double, 3>      Vector3d;
typedef TVector<double, 4>      Vector4d;
typedef TVector<int, 1>         Vector1i;
typedef TVector<int, 2>         Vector2i;
typedef TVector<int, 3>         Vector3i;
typedef TVector<int, 4>         Vector4i;
typedef TPoint<float, 1>        Point1f;
typedef TPoint<float, 2>        Point2f;
typedef TPoint<float, 3>        Point3f;
typedef TPoint<float, 4>        Point4f;
typedef TPoint<double, 1>       Point1d;
typedef TPoint<double, 2>       Point2d;
typedef TPoint<double, 3>       Point3d;
typedef TPoint<double, 4>       Point4d;
typedef TPoint<int, 1>          Point1i;
typedef TPoint<int, 2>          Point2i;
typedef TPoint<int, 3>          Point3i;
typedef TPoint<int, 4>          Point4i;


const float PI = static_cast<float>(acos(-1.0));



PHOENIX_NAMESPACE_END