#ifndef CORE_GRAPHICS_OBJECT_INTERFACE_H_
#define CORE_GRAPHICS_OBJECT_INTERFACE_H_

#include <string>

#include "../../../Graphics/typedefs.h"
#include "../../Maths/Transform.h"
#include "../../Utilities/SharedPointer.h"

namespace amit::graphics {
class IObjectController;
class Object {
 public:
  virtual Math::Transform getTransform() = 0;
  virtual void setTransform(
      Math::Vector3,
      Math::Quaternion = Math::Quaternion()) = 0;
  virtual void resetTransform() = 0;
  virtual void draw(bool) = 0;
  virtual bool isRenderable() const = 0;
  virtual bool isDebugObject() const = 0;
  virtual void updateObject() = 0;
  virtual void setObjectController(
      SharedPointer<IObjectController> objectController) = 0;
  virtual void setScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) = 0;
  virtual Math::Vector3 getScale() const = 0;
  virtual std::string getMaterialName() = 0;
  virtual void SetMaterial(std::string iMaterialName) = 0;
  virtual bool isCollidable() = 0;
  virtual bool isPhysicsEnabled() = 0;
  virtual void enableCollision(bool) = 0;
  virtual void enablePhysics(bool) = 0;
  virtual void castShadow(bool, Graphics::ShadowMethod) = 0;
  virtual bool castingShadowEnabled() = 0;
  virtual void receiveShadow(bool) = 0;
  virtual bool receivingShadowEnabled() = 0;
  virtual void setRenderable(bool) = 0;
  virtual bool IsPlayer() = 0;
  virtual void setPlayer(bool) = 0;
  virtual void setObjectType(ObjectType) = 0;
  virtual ObjectType getObjectType() = 0;
  virtual bool isSSAOEnabled() = 0;
  virtual void EnableSSAO(bool) = 0;
  virtual void SetVertexColor(float iR, float iG, float iB, float iA) = 0;
  virtual Graphics::RGBAColor GetVertexColor() const = 0;
  // virtual void ChangeString(std::string) = 0;
  Object() {}
  virtual ~Object() {}
};
}  

#endif