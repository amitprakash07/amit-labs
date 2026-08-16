#ifndef CORE_COMMON_CORE_OBJECTS_H
#define CORE_COMMON_CORE_OBJECTS_H

#include <map>

#include "additional_types.h"

namespace amit::common
{

    // Enum for type of Object property
    enum class ObjectProperty : uint8_t
    {
        Default = 0
    };

    // Interface IObjectPropertyAttributes class for describing object property
    // attributes
    class IObjectPropertyAttributes
    {
    public:
        virtual ~IObjectPropertyAttributes() = default;

        IObjectPropertyAttributes(const IObjectPropertyAttributes&)            = default;
        IObjectPropertyAttributes& operator=(const IObjectPropertyAttributes&) = default;

        IObjectPropertyAttributes(IObjectPropertyAttributes&&)            = default;
        IObjectPropertyAttributes& operator=(IObjectPropertyAttributes&&) = default;
    };

    // Interface class for all objects
    class IObject
    {
    public:
        virtual ~IObject()                                                                                                             = default;
        virtual void                       AddObjectPropertyAttributes(ObjectProperty property, IObjectPropertyAttributes* attributes) = 0;
        virtual IObjectPropertyAttributes* RemovePropertyAttributes(ObjectProperty property)                                           = 0;

        IObject(const IObject&)            = default;
        IObject& operator=(const IObject&) = default;

        IObject(IObject&&)            = default;
        IObject& operator=(IObject&&) = default;
    };

    // Default Object Property
    class ObjectDefaultPropertyAttributes : public IObjectPropertyAttributes
    {
    public:
        ObjectDefaultPropertyAttributes() = default;
    };

    // Default Object
    class CoreObject : public IObject
    {
    public:
        CoreObject()
            : default_attributes_()
        {
            object_property_attributes_map_[ObjectProperty::Default] = &default_attributes_;
        }

        ~CoreObject() = default;

    private:
        ObjectDefaultPropertyAttributes                      default_attributes_;
        std::map<ObjectProperty, IObjectPropertyAttributes*> object_property_attributes_map_;
    };
}  // namespace amit::common

#endif