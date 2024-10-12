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

        static int createContext(int width, int height);
        static void destroyContext(int id);

        static void setActive(int id);

        static Shared<Context> active;

        static uint8_t currentError;

    private:

        static ContextManager& getInstance();
        static ContextManager _instance;

        int m_firstFreeIdx = 0;

        std::array<Shared<Context>,MAX_CONTEXT_COUNT> m_contexts;
    };
}