#include "sgl.h"

#include "context/context_manager.h"
#include "context/context.h"
#include <iterator>
#include <memory>
#include <algorithm>
#include <new>

namespace sgl
{
    uint8_t ContextManager::currentError = 0;

    Shared<Context> ContextManager::active = nullptr;

    int ContextManager::createContext(int width, int height)
    {
        ContextManager& manager = getInstance();
        int& freeIdx = manager.m_firstFreeIdx;

        if (freeIdx >= MAX_CONTEXT_COUNT)
        {
            currentError = SGL_OUT_OF_RESOURCES;
            return -1;
        }

        int id = freeIdx;
        try 
        {
            Shared<Context> newContext = std::make_shared<Context>(width, height);
            manager.m_contexts[id] = newContext;
        }
        catch (const std::bad_alloc& e)
        {
            currentError = SGL_OUT_OF_MEMORY;
        }

        freeIdx = std::distance(manager.m_contexts.begin(), std::find_if(manager.m_contexts.begin(), manager.m_contexts.end(), [](const Shared<Context> c) { return c == nullptr; }));

        return id;
    }

    void ContextManager::destroyContext(int id)
    {
        ContextManager& manager = getInstance();
        int& freeIdx = manager.m_firstFreeIdx;

        if (id >= MAX_CONTEXT_COUNT || id < 0 || id == active->getId() || !manager.m_contexts[id] )
        {
            currentError = SGL_INVALID_VALUE;
            return;
        }
        manager.m_contexts[id] = nullptr;

        if (freeIdx >= id)
        {
            freeIdx = id;
        }
    }

    void ContextManager::setActive(int id)
    {
        ContextManager& manager = getInstance();
        if (id >= MAX_CONTEXT_COUNT || id < 0 || !manager.m_contexts[id] )
        {
            currentError = SGL_INVALID_VALUE;
            return;
        }

        if (active->isDrawing())
        {
            return;
        }

        active = manager.m_contexts[id];
    }

    int ContextManager::getActiveId()
    {
        if (!active)
        {
            currentError = SGL_INVALID_OPERATION;
            return -1;
        }
        return active->getId();
    }

    ContextManager& ContextManager::getInstance()
    {
        return _instance;
    }

} // namespace sgl
