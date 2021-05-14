//
// Created by jzy99 on 2021/5/13.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_PHOTON_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_PHOTON_H_

#include<phoenix/core/phoenix.h>
#include<phoenix/core/vector.h>
#include<phoenix/core/color.h>
#include<phoenix/core/kdtree.h>

PHOENIX_NAMESPACE_BEGIN

struct PhotonData {
  uint8_t rgbe[4];        //!< Photon power stored in Greg Ward's RGBE format
  uint8_t theta;            //!< Discretized photon direction (\a theta component)
  uint8_t phi;            //!< Discretized photon direction (\a phi component)

  /// Dummy constructor
  PhotonData() { }

  /// Initialize a photon with the specified direction of propagation and power
  PhotonData(const Vector3f &dir, const Color3f &power);

  /**
   * Convert the stored photon direction from quantized spherical
   * coordinates to a Vector3f value. Uses precomputation similar to
   * that of Henrik Wann Jensen's implementation.
   */
  Vector3f getDirection() const {
    return Vector3f(
        m_cosPhi[phi] * m_sinTheta[theta],
        m_sinPhi[phi] * m_sinTheta[theta],
        m_cosTheta[theta]
    );
  }

  /// Convert the photon power from RGBE to floating point
  Color3f getPower() const {
    return Color3f(rgbe[0], rgbe[1], rgbe[2]) * m_expTable[rgbe[3]];
  }
 protected:
  // ======================================================================
  /// @{ \name Precomputed lookup tables
  // ======================================================================

  static float m_cosTheta[256];
  static float m_sinTheta[256];
  static float m_cosPhi[256];
  static float m_sinPhi[256];
  static float m_expTable[256];
  static bool m_precompTableReady;

  /// @}
  // ======================================================================

  /// Initialize the precomputed lookup tables
  static bool initialize();
};

/// A Photon which also doubles as a kd-tree node for use with \ref PointKDTree
struct Photon : public GenericKDTreeNode<Point3f, PhotonData> {
 public:
  /// Dummy constructor
  Photon() { }

  /// Construct from a photon interaction
  Photon(const Point3f &pos, const Vector3f &dir, const Color3f &power)
      : GenericKDTreeNode(pos, PhotonData(dir, power)) { }

  Vector3f getDirection() const { return data.getDirection(); }
  Color3f getPower() const { return data.getPower(); }

  Photon operator/ (float k) const {
    Color3f scaled_power = (this->getPower()) / k;
    Photon ret(this->getPosition(), this->getDirection(), scaled_power);
    return ret;
  }
};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_PHOTON_H_
