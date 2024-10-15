#include "sgl.h"

#include "context/context_manager.h"
#include "context/context.h"
#include <memory>
#include <algorithm>
#include <iostream>

namespace sgl
{
    int ContextManager::createContext(int width, int height)
    {
        int& freeIdx = m_firstFreeIdx;

        if (m_firstFreeIdx >= MAX_CONTEXT_COUNT)
        {
            m_currentError = SGL_OUT_OF_RESOURCES;
            return -1;
        }

        int id = freeIdx;
        Context ctx(width, height, id);
        m_contexts[id] = std::move(ctx);
        m_firstFreeIdx = static_cast<int>(std::distance(m_contexts.begin(), std::find_if(m_contexts.begin(), m_contexts.end(), [](const Context& c) { return !c.isInitialized(); } )));

        return id;
    }

    void ContextManager::destroyContext(int id)
    {
        int& freeIdx = m_firstFreeIdx;

        if (id >= MAX_CONTEXT_COUNT || id < 0 || id == getActiveId() || !m_contexts[id].isInitialized())
        {
            m_currentError = SGL_INVALID_VALUE;
            return;
        }
        m_contexts[id] = Context();

        if (freeIdx >= id)
        {
            freeIdx = id;
        }
    }

    void ContextManager::setActive(int id)
    {
        if (id >= MAX_CONTEXT_COUNT || id < 0 || !m_contexts[id].isInitialized() )
        {
            m_currentError = SGL_INVALID_VALUE;
            return;
        }

        if (m_activeContextId >= 0 && m_contexts[m_activeContextId].isDrawing())
        {
            m_currentError = SGL_INVALID_OPERATION;
            return;
        }

        m_activeContextId = id;
    }

    Context* ContextManager::getActive()
    {
        if (!isActiveValid())
        {
            return nullptr;
        }
        return &m_contexts[m_activeContextId];
    }

    bool ContextManager::isActiveValid() const
    {
        return isContextValid(m_activeContextId);
    }

    int ContextManager::getActiveId() const 
    {
        return m_activeContextId;
    }

    bool ContextManager::isContextValid(int id) const
    {        
        return id >= 0 && id < MAX_CONTEXT_COUNT && m_contexts[id].isInitialized();
    }

    uint8_t ContextManager::getError()
    {
        uint8_t err = m_currentError;
        m_currentError = SGL_NO_ERROR;
        return err;
    }

    void ContextManager::setError(uint8_t errorCode)
    {
        
    }

    const char* ContextManager::getErrorString(uint8_t errorCode) const
    {
        return "";
    }

    ContextManager::ContextManager()
        : m_currentError(SGL_NO_ERROR),
        m_activeContextId(-1),
        m_firstFreeIdx(0)
    {
    }

    ContextManager& ContextManager::getInstance()
    {
        return s_instance;
    }

    Context& ContextManager::getActiveContext()
    {
        assert(m_activeContextId >= 0 && m_activeContextId < MAX_CONTEXT_COUNT);
        return m_contexts[m_activeContextId];
    }

} // namespace sgl
