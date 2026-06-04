#include <iostream>

#include "../../../Core/EngineCore/EngineCore.h"
#include "../../../Core/Utilities/HashedString.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Uniform.h"
#include "../../../Windows/WindowsFunctions.h"
#include "MeshObject.h"
#include "Scene.h"
#include "projection.h"
#include "transform.h"

SharedPointer<MeshObject> MeshObject::CreateMeshObject(
    std::string i_meshFileName, std::string i_materialName,
    Graphics::RGBAColor iColor) {
  i_meshFileName = EngineCore::getMeshFolderPath() + i_meshFileName;
  i_materialName = EngineCore::getMaterialFolderPath() + i_materialName;
  SharedPointer<MeshObject> tempMeshObject(
      new MeshObject(i_meshFileName, i_materialName), "MeshObject");
  utils::StringHash temp(
      EngineCore::getStringPool()->findString("UpdateObject"));
  EngineCore::getMessagingSystem()->addMessageHandler(
      temp, tempMeshObject.CastSharedPointer<IMessageHandler>(),
      typedefs::HIGH);

  if (!i_meshFileName.empty() && !i_meshFileName.empty() &&
      !i_materialName.empty()) {
    std::stringstream errormessage;

    if (!Graphics::Material::addMaterialToList(
            i_materialName.c_str())) {
      errormessage << "Unable to Load Effect";
      MessageBox(nullptr, errormessage.str().c_str(), nullptr, MB_OK);
      tempMeshObject.deleteObject();
      return (SharedPointer<MeshObject>());
    }

    if (!Graphics::Mesh::addToMeshList(i_meshFileName)) {
      errormessage << "Unable to Load Mesh";
      MessageBox(nullptr, errormessage.str().c_str(), nullptr, MB_OK);
      tempMeshObject.deleteObject();
      return (SharedPointer<MeshObject>());
    }

    // VertexColorModifier Uniform - Change here if changing the of the color;
    SharedPointer<Graphics::Uniform> colorUniform =
        Graphics::Uniform::addUniform(
            tempMeshObject->vertexModifierUniform,
            tempMeshObject->getMaterial()->getEffectName(), Graphics::Vertex);
    colorUniform->setValType(Graphics::Vector);
    colorUniform->setValCount(1);
    tempMeshObject->vertexColor = iColor;
    std::string tempUniformName = tempMeshObject->vertexModifierUniform;
    colorUniform->setHandle(
        tempMeshObject->getEffect()->Graphics::Effect::getUniformHandle(
            tempUniformName.c_str(), Graphics::Vertex));
  }
  return tempMeshObject;
}

MeshObject::MeshObject(std::string i_meshName,
                               std::string i_materialName) {
  renderable = true;
  mTransform = Math::Transform();
  mInitialTransform = Math::Transform();
  mMeshName = i_meshName;
  mMaterial = i_materialName;
  debugObject = false;
  vertexModifierUniform = "vertexColorModifier\0";
  isInitialTransform = true;
  scaleFactor = Math::Vector3(1.0, 1.0, 1.0);
  isPlayer = false;
  mObjectType = ObjectType::NONE;
  isSSAO = false;
}

MeshObject::MeshObject() {
  renderable = true;
  mMeshName = "";
  mMaterial = "";
  mTransform = Math::Transform();
  debugObject = false;
  isInitialTransform = true;
}

void MeshObject::EnableDebugging(bool enable) { debugObject = enable; }

bool MeshObject::isDebugObject() const { return debugObject; }

void MeshObject::setObjectController(
    SharedPointer<IObjectController> i_objectController) {
  if (!i_objectController.isNull()) mObjectController = i_objectController;
}

SharedPointer<Graphics::Mesh> MeshObject::getMesh()
    const {
  return Graphics::Mesh::getMesh(mMeshName);
}

SharedPointer<Graphics::Effect> MeshObject::getEffect()
    const {
  return Graphics::Effect::getEffect(
      Graphics::Material::getMaterial(mMaterial.c_str())
          ->getEffectName());
}

SharedPointer<Graphics::Material>
MeshObject::getMaterial() const {
  return Graphics::Material::getMaterial(mMaterial.c_str());
}

bool MeshObject::isRenderable() const { return renderable; }

void MeshObject::setTransform(Math::Vector3 i_positionOffset,
                                      Math::Quaternion i_orientation) {
  mTransform = Math::Transform(i_positionOffset, i_orientation);
  if (isInitialTransform) {
    mInitialTransform = mTransform;
    isInitialTransform = false;
  }
}

void MeshObject::resetTransform() { mTransform = mInitialTransform; }

void MeshObject::HandleMessage(utils::StringHash& i_message,
                                       SharedPointer<RTTI> i_MessageSender,
                                       void* i_pMessageData) {
  if (!i_MessageSender.isNull()) {
    if (!i_MessageSender.isNull() &&
        utils::StringHash("UpdateObject") == i_message &&
        !mObjectController.isNull())
      mObjectController->updateObject(
          *this, *reinterpret_cast<typedefs::ActionWithKeyBound*>(
                     i_pMessageData));
  }
}

void MeshObject::updateObject() {
  typedefs::ActionWithKeyBound action;
  action.action = typedefs::Default;
  action.keyVal = 0x00;
  if (!mObjectController.isNull())
    mObjectController->updateObject(*this, action);
}

void MeshObject::setScale(float x, float y, float z) {
  scaleFactor = Math::Vector3(x, y, z);
}

Math::Vector3 MeshObject::getScale() const {
  return scaleFactor;
}

Math::Transform MeshObject::getTransform() {
  return mTransform;
}

MeshObject::~MeshObject() {
  if (!mObjectController.isNull()) mObjectController.deleteObject();
}

void MeshObject::draw(bool drawDebugObject) {
  if (renderable && (drawDebugObject && debugObject || !debugObject)) {
    SharedPointer<Scene> activeScene = Scene::getRenderableScene();
    SharedPointer<Camera> tempCamera = activeScene->getActiveCamera();
    SharedPointer<Graphics::Material> tempMaterial =
        Graphics::Material::getMaterial(mMaterial.c_str());
    WindowsUtil::Assert(!tempMaterial.isNull(),
                        "Material for the skybox is not available");
    Scene::applyPaintersAlgorithmForTransparency();
    if (!tempCamera.isNull()) {
      Math::Transform cameraTransform = tempCamera->getTransform();
      float fieldOfView = tempCamera->getFieldOfView();
      float aspectRatio = tempCamera->getAspectRatio();
      float nearPlane = tempCamera->getNearPlane();
      float farPlane = tempCamera->getFarPlane();
      if (debugObject) Graphics::GraphicsSystem::EnableWireFrame(true);

      getEffect()->setShaders();

      Math::Transform gameObjectTransform = getTransform();
      std::string effectFile = getMaterial()->getEffectName();

      SharedPointer<Graphics::Uniform> localToWorldUniform =
          Graphics::Uniform::getUniform(
              getEffect()->getTransformMatrixUniformName(
                  Graphics::Vertex, Graphics::LocalToWorld),
              effectFile, Graphics::Vertex);

      SharedPointer<Graphics::Uniform> worldToView =
          Graphics::Uniform::getUniform(
              getEffect()->getTransformMatrixUniformName(Graphics::Vertex,
                                                         Graphics::WorldToView),
              effectFile, Graphics::Vertex);

      SharedPointer<Graphics::Uniform> viewToScreen =
          Graphics::Uniform::getUniform(
              getEffect()->getTransformMatrixUniformName(
                  Graphics::Vertex, Graphics::ViewToScreen),
              effectFile, Graphics::Vertex);

      SharedPointer<Graphics::Uniform> scaleMatrix =
          Graphics::Uniform::getUniform(
              getEffect()->getTransformMatrixUniformName(Graphics::Vertex,
                                                         Graphics::ScaleMatrix),
              tempMaterial->getEffectName(), Graphics::Vertex);

      SharedPointer<Graphics::Uniform> normalMatrix =
          Graphics::Uniform::getUniform(
              getEffect()->getTransformMatrixUniformName(
                  Graphics::Vertex, Graphics::NormalMatrix),
              tempMaterial->getEffectName(), Graphics::Vertex);

      if (!localToWorldUniform.isNull()) {
        Graphics::UniformValues localToWorlValues;
        localToWorlValues.matrixValue.Type = Graphics::LocalToWorld;
        localToWorlValues.matrixValue.matrix = Math::Matrix4x4(
            mTransform.getOrientation(), mTransform.getPosition());
        localToWorldUniform->setUniformValue(localToWorlValues);
      }

      if (!worldToView.isNull()) {
        Graphics::UniformValues worldToViewValues;
        worldToViewValues.matrixValue.Type = Graphics::WorldToView;
        worldToViewValues.matrixValue.matrix =
            amit::graphics::MakeWorldToViewTransform(
                cameraTransform.getOrientation(),
                cameraTransform.getPosition());
        worldToView->setUniformValue(worldToViewValues);
      }

      if (!scaleMatrix.isNull()) {
        Graphics::UniformValues scaleMatrixValues;
        scaleMatrixValues.matrixValue.Type = Graphics::ScaleMatrix;
        scaleMatrixValues.matrixValue.matrix =
            amit::graphics::MakeScaleMatrix(scaleFactor);
        scaleMatrix->setUniformValue(scaleMatrixValues);
      }

      if (!normalMatrix.isNull()) {
        Graphics::UniformValues normalMatrixValues;
        normalMatrixValues.matrixValue.Type = Graphics::NormalMatrix;
        normalMatrixValues.matrixValue.matrix =
            Math::Matrix4x4::CreateNormalMatrix(Math::Matrix4x4(
                mTransform.getOrientation(), mTransform.getPosition()));
        normalMatrix->setUniformValue(normalMatrixValues);
      }

      if (!viewToScreen.isNull()) {
        Graphics::UniformValues viewToScreenValues;
        viewToScreenValues.matrixValue.Type = Graphics::ViewToScreen;
        const amit::graphics::PerspectiveFrustum perspectiveFrustum{
            .fov_in_radians = fieldOfView,
            .aspect_ratio   = aspectRatio,
            .near_plane     = nearPlane,
            .far_plane      = farPlane};
        viewToScreenValues.matrixValue.matrix =
            amit::graphics::MakePerspectiveLH(perspectiveFrustum);
        viewToScreen->setUniformValue(viewToScreenValues);
      }

      Graphics::UniformValues tempColor;

      tempColor.vectorValue.x = vertexColor.r;
      tempColor.vectorValue.y = vertexColor.g;
      tempColor.vectorValue.z = vertexColor.b;
      tempColor.vectorValue.w = vertexColor.a;

      SharedPointer<Graphics::Uniform> vertexColorUiform =
          Graphics::Uniform::getUniform(vertexModifierUniform,
                                                effectFile, Graphics::Vertex);
      vertexColorUiform->setUniformValue(tempColor);

      Graphics::Uniform::setAllUniformToShaderObjects(effectFile);
      Graphics::UniformBlock::setAllUniformBlockForTheEffectInShader(
          effectFile);

      getMaterial()->setMaterialUniformParameters();
      getMaterial()->setTextureUniform();
      getMesh()->drawMesh();
      if (debugObject) Graphics::GraphicsSystem::EnableWireFrame(false);
    } else {
      std::stringstream errormessage;
      errormessage << "Camera is not iniitalized\n";
      WindowsUtil::Print(errormessage.str().c_str());
      return;
    }
  }
}

void MeshObject::castShadow(bool, Graphics::ShadowMethod) {
  // To do
}

bool MeshObject::castingShadowEnabled() {
  // to do
  return true;
}

void MeshObject::enablePhysics(bool) {
  // To do
}

void MeshObject::receiveShadow(bool) {
  // To do
}

bool MeshObject::receivingShadowEnabled() {
  // to do
  return false;
}

bool MeshObject::isCollidable() {
  // To do
  return false;
}

bool MeshObject::isPhysicsEnabled() {
  // to do
  return false;
}

void MeshObject::enableCollision(bool) {
  // to do
}

bool MeshObject::IsPlayer() { return isPlayer; }

void MeshObject::setPlayer(bool i_isPlayer) { isPlayer = i_isPlayer; }

ObjectType MeshObject::getObjectType() { return mObjectType; }

void MeshObject::setObjectType(ObjectType iObjectType) {
  mObjectType = iObjectType;
}

Graphics::RGBAColor MeshObject::GetVertexColor() const {
  return vertexColor;
}

void MeshObject::SetVertexColor(float iR, float iG, float iB,
                                        float iA) {
  vertexColor = Graphics::RGBAColor(iR, iG, iB, iA);
}

void MeshObject::SetMaterial(std::string iMaterialName) {
  mMaterial = iMaterialName;
}

void MeshObject::EnableSSAO(bool iRequest) { isSSAO = iRequest; }

bool MeshObject::isSSAOEnabled() { return isSSAO; }
