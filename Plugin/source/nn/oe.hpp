#pragma once

/* Minimal nn::oe declarations used by the plugin (dynamically linked against the game's nnSdk). */

#include <nn/nn_common.hpp>

namespace nn::oe {

    enum PerformanceMode {
        Invalid = -1,
        Normal,
        Boost,
    };

    void SetPerformanceConfiguration(PerformanceMode mode, s32 config);
}
