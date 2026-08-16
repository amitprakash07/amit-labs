#ifndef CORE_COMMON_CORE_H
#define CORE_COMMON_CORE_H

#include <string>
#include <atomic>
#include <format>

namespace amit::common
{
    struct ObjectId
    {
        uint64_t value() const
        {
            return id_;
        }

        explicit ObjectId(uint64_t id)
            : id_(id)
        {
        }

        bool operator==(const ObjectId& rhs) const
        {
            return id_ == rhs.id_;
        }

    private:
        uint64_t id_;
    };

    struct IdGenerator
    {
        static uint64_t GetNextId()
        {
            static std::atomic_uint64_t current_id_{0};
            ++current_id_;
            return current_id_;
        }
    };

    template <class ObjectType>
    concept ObjectTypeEnumConcept = std::is_enum_v<ObjectType> && requires(ObjectType enum_type) {
        { EnumTypeToString(enum_type) } -> std::convertible_to<std::string_view>;
    };

    template <ObjectTypeEnumConcept ObjectType>
    struct ObjectLabel
    {
        ObjectLabel(ObjectType type, const std::string& name)
            : object_name(name)
            , object_type(type)
            , object_id(IdGenerator::GetNextId())
        {
        }

        ObjectLabel(ObjectType type)
            : object_name()
            , object_type(type)
            , object_id(IdGenerator::GetNextId())
        {
        }

        ObjectLabel() = delete;

        void SetName(const std::string& name)
        {
            object_name = name;
        }

        std::string ToString() const
        {
            std::string ret_string = std::format("[Type:{}, Name: {}, Id: {}]",
                                                 EnumTypeToString(object_type),
                                                 object_name.empty() ? "None" : object_name,
                                                 object_id.value());
            return ret_string;
        }

        const ObjectId& GetId() const
        {
            return object_id;
        }

    private:
        std::string object_name;
        ObjectType  object_type;
        ObjectId    object_id;
    };

}  // namespace amit::common

template <>
struct std::hash<amit::common::ObjectId>
{
    std::size_t operator()(const amit::common::ObjectId& obj) const noexcept
    {
        return std::hash<uint64_t>{}(obj.value());
    }
};

#endif
