#pragma once

#include "common.hpp"

#define EXL_MODULE_NAME "ToCS1-ENX"

/* No EXL_DEBUG: logging is disabled. */
#define EXL_USE_FAKEHEAP

namespace exl::setting {
    /* How large the fake .bss heap will be (std::string temporaries in the text hooks). */
    constexpr size_t HeapSize = 0x20000;

    /* How large the JIT area will be for hooks (each hook takes 200 bytes). */
    constexpr size_t JitSize = 0x2000;

    /* How large the area will be inline hook pool. */
    constexpr size_t InlinePoolSize = 0x1000;

    /* How large the formatting buffer should be for logging. The buffer will be on the stack. */
    constexpr size_t LogBufferSize = 512;

    /* Sanity checks. */
    static_assert(ALIGN_UP(JitSize, PAGE_SIZE) == JitSize, "");
    static_assert(ALIGN_UP(InlinePoolSize, PAGE_SIZE) == InlinePoolSize, "");
}
