#pragma once

#ifndef CORE_GRAPHICS_DEBUG_SHAPES_H_
#define CORE_GRAPHICS_DEBUG_SHAPES_H_

#define CORE_GRAPHICS_DEBUG_MODE
#include "../../Graphics/Line.h"

#ifdef CORE_GRAPHICS_DEBUG_MODE

#include "../../Graphics/typedefs.h"
#include "../EngineCore/Objects/MeshObject.h"
#include "../Utilities/SharedPointer.h"

namespace amit::graphics {
enum ShapeTarget {
  SPHERE = 0,    // Needs radius,position
  BOX = 1,       // Need Position, length, breadth,height
  CYLINDER = 2,  // Need position, radius, height
  LINE = 3       // Need two positions
};
namespace Debug {
class DebugShapes {
 public:
  static bool IsDebugShapesAvailable();
  static SharedPointer<Object> GetDebugShape(std::string iName,
                                             ShapeTarget iShape);
  static SharedPointer<MeshObject> DrawShape(
      std::string iName, ShapeTarget, Math::Vector3,
      float iRadius = 1.0f,
      Graphics::RGBColor = Graphics::RGBColor(0.0f, 1.0f,
                                                              0.0f));  // Sphere

  static SharedPointer<MeshObject> DrawShape(
      std::string iName, ShapeTarget, Math::Vector3,
      float iLength = 1.0f, float iWidth = 1.0f, float iHeight = 1.0f,
      Graphics::RGBColor = Graphics::RGBColor(0.0f, 1.0f,
                                                              0.0f));  // Box

  static SharedPointer<MeshObject> DrawShape(
      std::string iName, ShapeTarget, Math::Vector3,
      float iRadius = 1.0f, float iHeight = 1.0f,
      Graphics::RGBColor =
          Graphics::RGBColor(0.0f, 1.0f, 0.0f));  // Cylinder

  static SharedPointer<Graphics::Line> DrawShape(
      std::string iName, ShapeTarget, Math::Vector3,
      Math::Vector3,
      Graphics::RGBColor = Graphics::RGBColor(0.0f, 1.0f,
                                                              0.0f));  // Line
 private:
  DebugShapes();
  static std::map<ShapeTarget,
                  std::map<std::string, SharedPointer<Object>>>
      debugShapeList;
  SharedPointer<Object> debugShape;
};
}  // namespace Debug
}  
#endif

#endif