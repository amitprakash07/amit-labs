#ifndef CORE_COMMON_MESSAGING_SYSTEM_H
#define CORE_COMMON_MESSAGING_SYSTEM_H

#include <map>
#include <vector>

#include "rtti.h"
#include "hashed_string.h"
#include "message_handler_interface.h"

namespace amit::common
{
    class MessagingSystem : public RTTI
    {
    public:
        static SharedPointer<MessagingSystem> getMessagingSystem();
        void                                  addMessageHandler(utils::StringHash&, SharedPointer<IMessageHandler>, typedefs::Priority);
        bool                                  removeMessageHandler(utils::StringHash&, SharedPointer<IMessageHandler>);
        ~MessagingSystem();

        // template<typename T>
        void        sendMessage(utils::StringHash&, SharedPointer<RTTI>, void* message_data = nullptr);
        std::string GetType() const override
        {
            return "";
        }
        bool IsSameType(SharedPointer<RTTI>, std::string) const override
        {
            return true;
        }

    private:
        static SharedPointer<MessagingSystem>                   mMessagingSystem;
        std::map<utils::StringHash, std::vector<MessageHandle>> m_MessageHandlers;
        MessagingSystem();
    };
}  // namespace amit::common

#endif