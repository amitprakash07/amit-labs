#include <assert.h>

#include <algorithm>

#include "messaging_system.h"

using namespace amit::common;

SharedPointer<MessagingSystem> MessagingSystem::mMessagingSystem;

SharedPointer<MessagingSystem> MessagingSystem::getMessagingSystem()
{
    if (mMessagingSystem.isNull())
    {
        /*mMessagingSystem = EngineController::Gameamit::common::isEngineInitialized() ?
            new(EngineController::Gameamit::common::getMemoryManager()->__alloc(sizeof(MessagingSystem)))
       MessagingSystem() : new MessagingSystem();*/

        SharedPointer<MessagingSystem> temp(SharedPointer<MessagingSystem>(new MessagingSystem(), "MessagingSystem"));
        mMessagingSystem = temp;
    }
    return mMessagingSystem;
}

MessagingSystem::MessagingSystem()
{
    // Nothing Required
}

MessagingSystem::~MessagingSystem()
{
    // delete mMessagingSystem;
}

void MessagingSystem::addMessageHandler(utils::StringHash& i_message, SharedPointer<IMessageHandler> i_pMessageHandler, typedefs::Priority i_priority)
{
    // assert(!i_pMessageHandler.isNull());
    assert(!i_message.isNil());
    MessageHandle temp{i_pMessageHandler, i_priority};
    m_MessageHandlers[i_message].push_back(temp);
    std::vector<MessageHandle> tempList = m_MessageHandlers[i_message];
    std::sort(tempList.begin(), tempList.end());
}

void MessagingSystem::sendMessage(utils::StringHash& i_message, SharedPointer<RTTI> i_messageSender, void* message_data)
{
    // assert(i_messageSender!=nullptr); //!= nullptr);
    assert(!i_message.isNil());
    std::vector<MessageHandle> tempList    = m_MessageHandlers[i_message];
    size_t                     handleCount = tempList.size();
    for (size_t i = 0; i < handleCount; i++)
    {
        tempList[i].mHandler->HandleMessage(i_message, i_messageSender, message_data);
    }
}

bool MessagingSystem::removeMessageHandler(utils::StringHash& i_message, SharedPointer<IMessageHandler> i_messageHandler)
{
    // MessagedAssert(i_messageHandler != nullptr, "Null Message Handler - Removal
    // not possible");
    std::vector<MessageHandle> temp = m_MessageHandlers[i_message];
    for (unsigned __int16 i = 0; i < temp.size(); i++)
    {
        if (temp[i].mHandler.isEqual(i_messageHandler))
        {
            temp.erase(temp.begin() + i);
            return true;
        }
    }
    return false;
}
