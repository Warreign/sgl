#pragma once

#include "utils/common.h"
#include "context/context.h"

#include <array>

namespace sgl 
{
    class ContextManager
    {
    public:
        static inline const uint8_t MAX_CONTEXT_COUNT = 32;

        int createContext(int width, int height);
        void destroyContext(int id);

        void setActive(int id);
        Unique<Context> getActive();
        int getActiveId();

        uint8_t getError();
        void setError(uint8_t errorCode);
        const std::string& getErrorString(uint8_t errorCode);

        ContextManager();

        static ContextManager& getInstance();
        
    private:

        static ContextManager s_instance;

        Context& getActiveContext(); 

        uint8_t m_currentError;
        int m_firstFreeIdx;
        int m_activeContextId;
        std::array<Context, MAX_CONTEXT_COUNT> m_contexts;
    };
}