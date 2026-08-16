#ifndef ENGINE_PHYSICS_PHYSICS_SYSTEM_H_
#define ENGINE_PHYSICS_PHYSICS_SYSTEM_H_

#ifdef PLATFORM_OPEN_GL
#include "../../../Externals/OpenGLExtensions/OpenGlExtensions.h"
#elif PLATFORM_D3D
#include <d3d9.h>
#endif

#include "common/shared_pointer.h"
#include "core/maths/geometry/ray_intersection.h"
#include "../../Graphics/Line.h"
#include "../../Graphics/Mesh.h"
#include "../EngineCore/Objects/MeshObject.h"



using namespace amit::common;

namespace amit {
namespace Physics {
class HitInfo;

class PhysicsSystem {
 public:
  static void InitializePhysicsSystem(std::string iCollisionMesh);
  static SharedPointer<PhysicsSystem> GetPhysicsSystem();
  static bool ShowCollisionMesh();
  static void DrawCollisionMesh();
  static void SetShowCollisionMesh(bool iRequest);
  static void EnableCollisionDebugging(bool iRequest);
  static bool CollisionDebuggingStatus();
  bool CheckCollision(Physics::Ray iRay, HitInfo& iHitInfo) const;
  bool CheckCollision(Math::Vector3 startPoint, Math::Vector3 endPoint,
                      HitInfo& iHitInfo);
  bool IntersectTriangle(Math::Vector3 startPoint, Math::Vector3 EndPoint,
                         Triangle iTriangle, HitInfo& iHitInfo) const;

 private:
  static SharedPointer<PhysicsSystem> mPhysicsSystem;
  SharedPointer<Graphics::Mesh> mCollisionMesh;
  SharedPointer<MeshObject> mMeshObject;
  SharedPointer<Graphics::Line> AB;
  SharedPointer<Graphics::Line> BC;
  SharedPointer<Graphics::Line> AC;
  std::string collisionMeshName;
  bool showCollisionMesh;
  bool collisionDebugging;
  PhysicsSystem();
};
}  // namespace Physics
}  

#endif