
#include "engine_core.h"
#include "common/hashed_string.h"
#include "core/maths/linear_algebra/utils.h"
#include "input/../../Utilities/IObjectController.h"

#include "camera.h"

using namespace amit::graphics;

SharedPointer<Camera> Camera::CreateCamera(
    std::string i_name, Math::Vector3 i_position,
    Math::Quaternion i_orientation, float i_fieldofView, float i_aspectRatio) {
  SharedPointer<Camera> tempCamera(
      new Camera(i_name, i_position, i_orientation, i_fieldofView,
                         i_aspectRatio),
      "Camera");
  utils::StringHash temp(
      EngineCore::getStringPool()->findString("UpdateObject"));
  utils::StringHash tempOne(
      EngineCore::getStringPool()->findString("ActionOnMouseEvent"));
  EngineCore::getMessagingSystem()->addMessageHandler(
      temp, tempCamera.CastSharedPointer<IMessageHandler>(),
      typedefs::HIGH);
  EngineCore::getMessagingSystem()->addMessageHandler(
      tempOne, tempCamera.CastSharedPointer<IMessageHandler>(),
      typedefs::HIGH);
  return tempCamera;
}

Math::Vector3 Camera::getForwardVector() const {
  return (mTransform.getForwardVector());
}

Camera::Camera() {
  mCameraName = "";
  mTransform.setOrientation(Math::Quaternion());
  mTransform.setPosition(Math::Vector3());
  fieldOfView = 0.0f;
  aspectRatio = 0.0f;
  active = false;
  nearPlane = 0.0f;
  farPlane = 0.0f;
  mInitialTransform = Math::Transform();
  isInitialTransform = true;
}

Camera::Camera(std::string i_name, Math::Vector3 i_position,
                       Math::Quaternion i_orientation, float i_fieldOfView,
                       float i_aspectRatio, float i_nearPlane,
                       float i_farPlane) {
  mTransform = Math::Transform();
  mCameraName = i_name;
  mTransform.setOrientation(i_orientation);
  mTransform.setPosition(i_position);
  fieldOfView = Math::ConvertDegreesToRadians(i_fieldOfView);
  aspectRatio = i_aspectRatio;
  active = false;
  nearPlane = i_nearPlane;
  farPlane = i_farPlane;
  isInitialTransform = true;
  isPlayer = false;
  mObjectType = ObjectType::NONE;
}

void Camera::setFieldOfView(float i_fieldOfView) {
  fieldOfView = Math::ConvertDegreesToRadians(i_fieldOfView);
}

float Camera::getFieldOfView() const { return fieldOfView; }

void Camera::setAspectRatio(float i_aspectRatio) {
  aspectRatio = i_aspectRatio;
}

float Camera::getAspectRatio() const { return aspectRatio; }

void Camera::activateCamera(bool i_activate) { active = i_activate; }

bool Camera::isActive() const { return active; }

Math::Transform Camera::getTransform() { return mTransform; }

void Camera::setTransform(Math::Vector3 i_positionOffset,
                                  Math::Quaternion i_orientation) {
  mTransform = Math::Transform(i_positionOffset, i_orientation);
  if (isInitialTransform) {
    isInitialTransform = false;
    mInitialTransform = mTransform;
  }
}

void Camera::resetTransform() { mTransform = mInitialTransform; }

float Camera::getNearPlane() const { return nearPlane; }

float Camera::getFarPlane() const { return farPlane; }

std::string Camera::getCameraName() const { return mCameraName; }

void Camera::setFarPlane(float i_farPlane) { farPlane = i_farPlane; }

void Camera::setNearPlane(float i_nearPlane) {
  nearPlane = i_nearPlane;
}

void Camera::HandleMessage(utils::StringHash& i_message,
                                   SharedPointer<RTTI> i_MessageSender,
                                   void* i_pMessageData) {
  if (!i_MessageSender.isNull() &&
      utils::StringHash("UpdateObject") == i_message &&
      !mObjectController.isNull())
    mObjectController->updateObject(
        *this, *reinterpret_cast<typedefs::ActionWithKeyBound*>(
                   i_pMessageData));
}

void Camera::updateObject() {
  typedefs::ActionWithKeyBound action;
  action.action = typedefs::Default;
  action.keyVal = 0x00;
  if (!mObjectController.isNull())
    mObjectController->updateObject(*this, action);
}

void Camera::setObjectController(
    SharedPointer<IObjectController> i_objectController) {
  if (!i_objectController.isNull()) mObjectController = i_objectController;
}

bool Camera::isCollidable() { return false; }

bool Camera::isPhysicsEnabled() { return false; }

void Camera::enableCollision(bool) {
  // Stub
}

void Camera::enablePhysics(bool) {
  // Stub
}

void Camera::receiveShadow(bool) {
  // Stub
}

void Camera::castShadow(bool, Graphics::ShadowMethod) {
  // stub
}

bool Camera::castingShadowEnabled() { return false; }

bool Camera::receivingShadowEnabled() { return false; }

bool Camera::IsPlayer() { return isPlayer; }

void Camera::setPlayer(bool i_isPlayer) { isPlayer = i_isPlayer; }

ObjectType Camera::getObjectType() { return mObjectType; }

void Camera::setObjectType(ObjectType iObjectType) {
  mObjectType = iObjectType;
}
