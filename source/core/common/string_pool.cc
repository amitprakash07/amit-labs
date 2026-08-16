#include "hashed_string.h"
#include "string_pool.h"

using namespace amit::common;

SharedPointer<StringPool> StringPool::string_pool_instance_;

SharedPointer<StringPool> StringPool::Instance()
{
    if (string_pool_instance_.isNull())
    {
        /*string_pool_instance_ = EngineController::Gameamit::common::isEngineInitialized() ?
            new
       (EngineController::Gameamit::common::getMemoryManager()->__alloc(sizeof(StringPool)))
       StringPool() :
    */
        SharedPointer<StringPool> tempPool(new StringPool(), "StringPool");
        string_pool_instance_ = tempPool;
    }
    return string_pool_instance_;
}  // Instance

StringPool::~StringPool()
{
    while (string_list_.size() != 0)
    {
        delete (string_list_.at(0).mString);
        string_list_.at(0).mString = nullptr;
        string_list_.erase(string_list_.begin());
    }
}

StringPool::StringPool()
{
    string_list_.reserve(200);
}

char* StringPool::FindString(const char* i_string)
{
    if (i_string != nullptr)
    {
        size_t tempStringLength = strlen(i_string);
        bool   isFound          = false;

        for (size_t i = 0; i < string_list_.size(); i++)
        {
            if (utils::StringHash(string_list_[i].getAsChar()) == utils::StringHash(i_string))
                return string_list_[i].getAsChar();
        }
        return (AddString(i_string));
    }
    return nullptr;
}  // end find function

char* StringPool::AddString(const char* i_String)
{
    typedefs::String tempString = typedefs::String(i_String);
    string_list_.push_back(tempString);
    return tempString.mString;
}
