#ifndef CORE_GRAPHICS_MESH_OBJECT_H_
#define CORE_GRAPHICS_MESH_OBJECT_H_

#include "../../../Core/Utilities/IMessageHandler.h"
#include "../../../Core/Utilities/SharedPointer.h"
#include "../../../Graphics/Material.h"
#include "../../../Graphics/Mesh.h"
#include "../../../Graphics/typedefs.h"
#include "../../Utilities/IObjectController.h"
#include "Object.h"

namespace amit::graphics {
class MeshObject : public IMessageHandler, Object {
 public:
  static SharedPointer<MeshObject> CreateMeshObject(
      std::string, std::string,
      Graphics::RGBAColor = {1.0f, 1.0f, 1.0f, 1.0f});
  void setObjectController(SharedPointer<IObjectController>) override;
  SharedPointer<Graphics::Mesh> getMesh() const;
  SharedPointer<Graphics::Effect> getEffect() const;
  SharedPointer<Graphics::Material> getMaterial() const;
  void setTransform(
      Math::Vector3,
      Math::Quaternion = Math::Quaternion()) override;
  Math::Transform getTransform() override;
  bool isRenderable() const override;
  void HandleMessage(utils::StringHash&,
                     SharedPointer<RTTI> i_MessageSender,
                     void* i_pMessageData) override;
  void draw(bool drawDebugObject) override;
  bool IsPlayer() override;
  void setPlayer(bool i_isPlayer) override;
  std::string getTypeInfo() const override { return ""; }
  bool isBothSameType(SharedPointer<RTTI>, std::string) const override {
    return true;
  }
  void EnableDebugging(bool enable = false);
  bool isDebugObject() const override;
  void updateObject() override;
  void setScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) override;
  Math::Vector3 getScale() const override;
  void setRenderable(bool iRenderable) override { renderable = iRenderable; }
  void resetTransform() override;
  std::string getMaterialName() override { return mMaterial; }
  bool isCollidable() override;
  bool isPhysicsEnabled() override;
  void enableCollision(bool) override;
  void enablePhysics(bool) override;
  void castShadow(bool, Graphics::ShadowMethod) override;
  bool castingShadowEnabled() override;
  void receiveShadow(bool) override;
  bool receivingShadowEnabled() override;
  void setObjectType(ObjectType iObjectType) override;
  ObjectType getObjectType() override;
  Graphics::RGBAColor GetVertexColor() const override;
  void SetVertexColor(float iR, float iG, float iB, float iA) override;
  void SetMaterial(std::string iMaterialName) override;
  bool isSSAOEnabled() override;
  void EnableSSAO(bool iRequest) override;
  ~MeshObject();
  MeshObject();

 private:
  MeshObject(std::string, std::string);
  std::string mMeshName;
  std::string mMaterial;
  std::string vertexModifierUniform;
  SharedPointer<IObjectController> mObjectController;
  bool renderable;
  Math::Transform mTransform;
  Math::Transform mInitialTransform;
  bool isInitialTransform;
  Math::Vector3 scaleFactor;
  bool debugObject;
  Graphics::RGBAColor vertexColor;
  bool isPlayer;
  ObjectType mObjectType;
  bool isSSAO;
};
}  

#endif