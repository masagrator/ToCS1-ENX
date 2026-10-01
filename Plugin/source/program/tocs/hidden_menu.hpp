#pragma once

/* Settings menu (hold R on the title screen) and the hooks it controls. Port of Tfoaf/Hiddenmenu.hpp. */

#include "common.hpp"

inline int8_t indicator = 0;
inline uint8_t options_count = 3;

struct Resolution { uint32_t width, height; const char* label; };

/* Index 4 is the game's default. */
inline Resolution GetResolution(int setting) {
    switch (setting) {
        case 0:  return {640, 360, "[640x360]"};
        case 1:  return {853, 480, "[853x480]"};
        case 2:  return {960, 540, "[960x540]"};
        case 3:  return {1120, 630, "[1120x630]"};
        case 5:  return {1440, 810, "[1440x810]"};
        case 6:  return {1600, 900, "[1600x900]"};
        case 7:  return {1760, 990, "[1760x990]"};
        case 8:  return {1920, 1080, "[1920x1080]"};
        default: return {1280, 720, "[1280x720] (Default)"};
    }
}

/* Render target creation; only the 3D world target (called from 0x14D808) is resized. */
HOOK_DEFINE_TRAMPOLINE(TextureMaker) {
    static uint64_t Callback(void* x0, void* x1, uint32_t width, uint32_t height, void* x4, void* x5, void* x6, void* x7) {
        ptrdiff_t offsetItr = returnInstructionOffset((uintptr_t)__builtin_return_address(0));
        if (offsetItr == 0x14D808) {
            Resolution r = GetResolution(Settings.RenderingRes);
            return Orig(x0, x1, r.width, r.height, x4, x5, x6, x7);
        }
        return Orig(x0, x1, width, height, x4, x5, x6, x7);
    }
};

HOOK_DEFINE_TRAMPOLINE(RenderingRes) {
    static uint64_t Callback(void* unk, uint32_t width, uint32_t height) {
        Resolution r = GetResolution(Settings.RenderingRes);
        return Orig(unk, r.width, r.height);
    }
};

HOOK_DEFINE_TRAMPOLINE(FPSlock) {
    static uint64_t Callback(void* unk, uint32_t FPStarget) {
        static void* unk_holder = 0;
        if (unk != nullptr)
            unk_holder = unk;
        else
            unk = unk_holder;
        if (Settings.FPS == 30)
            return Orig(unk, FPStarget);
        else
            return Orig(unk, Settings.FPS);
    }
};