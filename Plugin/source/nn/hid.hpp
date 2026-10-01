#pragma once

/* Minimal nn::hid declarations used by the plugin (dynamically linked against the game's nnSdk). */

#include <nn/nn_common.hpp>

namespace nn::hid {

    enum HidControllerKeys : u64 {
        KEY_A     = BIT(0),
        KEY_R     = BIT(7),
        KEY_DUP   = BIT(13),
        KEY_DDOWN = BIT(15),
    };

    enum NpadId : u32 {
        CONTROLLER_PLAYER_1 = 0,
        CONTROLLER_HANDHELD = 0x20,
    };

    struct NpadHandheldState {
        s64 updateCount;
        u64 Buttons;
        s32 LStickX;
        s32 LStickY;
        s32 RStickX;
        s32 RStickY;
        u32 Flags;
    };

    /* Same layout, different type (and so different overload) in the SDK. */
    struct NpadFullKeyState : NpadHandheldState {};
    struct NpadJoyDualState : NpadHandheldState {};

    void GetNpadState(NpadHandheldState* out, u32 const& id);
    void GetNpadState(NpadFullKeyState* out, u32 const& id);
    void GetNpadState(NpadJoyDualState* out, u32 const& id);
}
