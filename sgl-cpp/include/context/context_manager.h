#pragma once

#include "context/context.h"

#include <optional>
#include <array>

namespace sgl 
{
    class SglController
    {
    public:
        static inline const uint8_t MAX_CONTEXT_COUNT = 32;

        int createContext(int width, int height);
        void destroyContext(int id);

        void setActive(int id);
        Context* getActive();
        int getActiveId() const;
        bool isContextValid(int id) const;
        bool isActiveValid() const;

        uint8_t getError();
        void setError(uint8_t errorCode);
        const char* getErrorString(uint8_t errorCode) const;

        SglController();

        static SglController& getInstance();
        
    private:

        static SglController s_instance;

        Context& getActiveContext(); 

        uint8_t m_currentError;
        int m_firstFreeIdx;
        int m_activeContextId;
        std::array<Context, MAX_CONTEXT_COUNT> m_contexts;
    };
}