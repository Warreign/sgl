#pragma once

#include <cstdint>

namespace sgl
{
    
class Context
{

public:
    Context() = delete;
    Context(Context& /* other */) = delete;

    explicit Context(int width, int height);

private:
    const uint32_t m_id;

    const uint32_t m_width;
    const uint32_t m_height;

};

} // namespace sgl
